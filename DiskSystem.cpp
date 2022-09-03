#include "DiskSystem.h"
#include <fstream>

bool DiskFile::IsStale() const
{
	const long long now = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	const long long nano = Timestamp.time_since_epoch().count();
	if((now - nano) > GcNanoseconds)
	{
		return true;
	}
	return false;
}

std::vector<unsigned char> DiskFile::GetData()
{
	std::scoped_lock lock(Mut);
	return Data;
}

DiskSystem::DiskSystem()
{
	FileCache = {};
	Root = std::filesystem::current_path();
	bUseRootDirectory = true;
	bDestroyed = false;
	GcThread = std::thread([&]()
		{
			while (!bDestroyed)
			{
				std::this_thread::sleep_for(std::chrono::seconds(60));
				GarbageCollect();
			}
		});
	GcThread.detach();

}

DiskSystem::DiskSystem(const std::string& rootPath)
{
	FileCache = {};
	bUseRootDirectory = false;
	bDestroyed = false;
	GcThread = std::thread([&]()
	{
			while (!bDestroyed)
			{
				std::this_thread::sleep_for(std::chrono::seconds(60));
				GarbageCollect();
			}
	});
	GcThread.detach();

	if (std::filesystem::exists(rootPath))
	{
		Root = std::filesystem::path(rootPath);
		bUseRootDirectory = true;
		return;
	}

}

DiskSystem::~DiskSystem()
{
	std::scoped_lock lock(GcMut, CacheMut);
	bDestroyed = true;
	FileCache.clear();
}

std::shared_ptr<DiskFile> DiskSystem::GetFile(const std::string& path)
{
	std::scoped_lock lock(CacheMut);

	std::filesystem::path finalPath;

	if(bUseRootDirectory)
		finalPath = Root;

	finalPath += path;

	// Try cached
	if(!FileCache.empty())
	{
		if (FileCache.contains(finalPath.string()))
		{
			const auto cachedFile = FileCache.find(finalPath.string());
			// update when the file data was last accessed
			cachedFile->second->Timestamp = std::chrono::high_resolution_clock::now();
			return cachedFile->second;
		}

	}
	
	// cache doesnt exist, Try load file into cache

	if (std::filesystem::exists(finalPath))
	{
		auto diskFile = std::make_shared<DiskFile>();
		auto diskFileRet = std::make_shared<DiskFile>();
		diskFile->Path = finalPath;
		auto time = std::chrono::high_resolution_clock::now();
		diskFile->Timestamp = std::chrono::high_resolution_clock::now();
		std::ifstream fileStream(finalPath, std::ios::binary | std::ios::ate);

		fileStream.seekg(0, std::ios::end);
		const std::streampos fileSize = fileStream.tellg();
		fileStream.seekg(0, std::ios::beg);

		diskFile->Size = fileSize;
		diskFile->Data.reserve(fileSize);

		diskFile->Data.insert(diskFile->Data.begin(),
			std::istream_iterator<BYTE>(fileStream),
			std::istream_iterator<BYTE>());


		FileCache.try_emplace(finalPath.string(), diskFile);
		return diskFile;
	}
	return nullptr;

}

void DiskSystem::SetRootDirectory(const std::string& rootPath)
{
	Root = std::filesystem::path(rootPath);
}

void DiskSystem::SetUseRootDirectory(bool bUseRootDir)
{
	bUseRootDirectory = bUseRootDir;
}

void DiskSystem::GarbageCollect()
{
	std::scoped_lock lock(GcMut);
	if (FileCache.empty())
	{
		return;
	}
	for (const auto& pair : FileCache)
	{
		auto file = pair.second;

		std::scoped_lock file_lock(file->Mut);
		// maybe broken??
		if (file.use_count() <= 1)
		{
			std::erase_if(FileCache, [](const std::pair<std::string, std::shared_ptr<DiskFile>>& fpair)
				{
					return fpair.second->IsStale();
				});
		}
	}
};