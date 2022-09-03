#include "DiskSystem.h"
#include <fstream>
#include "Logger.h"

bool DiskFile::IsStale() const
{
	const long long now = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	const long long nano = Timestamp.time_since_epoch().count();
	if((now - nano) > GcNanoseconds)
	{
		Logger::LogInfo("DiskFile::IsStale()", "GC Freeing cached file: " + Path.string());
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
				std::this_thread::sleep_for(std::chrono::seconds(GARBAGE_COLLECTOR_THREAD_WAIT));
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
				std::this_thread::sleep_for(std::chrono::seconds(GARBAGE_COLLECTOR_THREAD_WAIT));
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

std::shared_ptr<DiskFile> DiskSystem::GetFileCached(const std::string& path)
{
	std::scoped_lock lock(CacheMut, GcMut);

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
			Logger::LogInfo("DiskSystem::GetFileCached()", "Returning cached file: " + cachedFile->second->Path.string());
			return cachedFile->second;
		}

	}
	
	// cache doesnt exist, Try load file into cache

	if (std::filesystem::exists(finalPath))
	{
		auto diskFile = std::make_shared<DiskFile>();
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
		Logger::LogInfo("DiskSystem::GetFileCached()", "Loaded file into cache: " + diskFile->Path.string());
		return diskFile;
	}
	return nullptr;

}

std::vector<std::string> DiskSystem::EnumDirectory(const std::string& path, bool bRecursive)
{
	std::string finalPath = path;
	if (bUseRootDirectory)
	{
		finalPath = Root.string() + path;
	}
	std::vector<std::string> filesFound;
	if(!bRecursive)
	{
		for (const auto& entry : std::filesystem::directory_iterator(finalPath))
		{
			filesFound.push_back(entry.path().string());
		}
	}
	else
	{
		for (const auto& entry : std::filesystem::recursive_directory_iterator(finalPath))
		{
			filesFound.push_back(entry.path().string());
		}
	}
	return filesFound;
}

std::vector<std::string> DiskSystem::EnumDirectory(const std::string& path, const std::string& extension, bool bRecursive)
{
	std::string finalPath = path;
	if(bUseRootDirectory)
	{
		finalPath = Root.string() + path;
	}
	std::vector<std::string> filesFound;
	if (!bRecursive)
	{
		for (const auto& entry : std::filesystem::directory_iterator(finalPath))
		{
			if(entry.path().extension() == extension)
			{
				filesFound.push_back(entry.path().string());
			}
		}
	}
	else
	{
		for (const auto& entry : std::filesystem::recursive_directory_iterator(finalPath))
		{
			if (entry.path().extension() == extension)
			{
				filesFound.push_back(entry.path().string());
			}
		}
	}
	return filesFound;
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
	std::scoped_lock lock(GcMut, CacheMut); //Refactor into one mutex?
	if (FileCache.empty())
	{
		return;
	}
	std::erase_if(FileCache, [](const std::pair<std::string, std::shared_ptr<DiskFile>>& fpair)
		{
			if(fpair.second.use_count() <= 2) // using this increments it by 1 so we use <=2
				return fpair.second->IsStale();
			return false;
		});
}