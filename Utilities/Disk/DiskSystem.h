#pragma once
#include <Windows.h>
#include <memory>
#include <vector>
#include <filesystem>
#include <string>
#include <unordered_map>
#include <mutex>
#include <chrono>
#include <future>

constexpr long long STALE_FILE_SECONDS = 60;
constexpr long long STALE_FILE_NANOSECONDS = STALE_FILE_SECONDS * 1000000000;


struct DiskFile
{
	
	std::filesystem::path Path;
	size_t Size;

	std::vector<unsigned char> GetData();
	std::string& GetTextData();
	bool IsText() const;

	friend class DiskSystem;
private:
	bool IsStale() const;
	std::chrono::time_point<std::chrono::steady_clock> Timestamp;
	std::mutex Mut = {};
	long long GcNanoseconds = STALE_FILE_NANOSECONDS;
	bool bIsText;
	std::vector<BYTE> Data;
	std::string TextData;
};

class DiskSystem
{
public:
	DiskSystem();
	DiskSystem(const std::string& rootPath);
	~DiskSystem();
	std::shared_ptr<DiskFile> GetFileCached(const std::string &path, bool bTextFile);
	std::shared_ptr<DiskFile> GetFile(const std::string& path, bool bTextFile);

	std::future<std::shared_ptr<DiskFile>> GetFileCachedAsync(const std::string& path, bool bTextFile);
	std::future<std::shared_ptr<DiskFile>> GetFileAsync(const std::string& path, bool bTextFile);

	std::vector<std::string> EnumDirectory(const std::string& path, bool bRecursive);
	std::vector<std::string> EnumDirectory(const std::string& path, const std::string& extension, bool bRecursive);

	void SetRootDirectory(const std::string& rootPath);
	void SetUseRootDirectory(bool bUseRootDir);
	void GarbageCollect();
private:
	std::mutex DiskMut;
	bool bUseRootDirectory;
	std::filesystem::path Root;
	std::unordered_map <std::string, std::shared_ptr<DiskFile>> FileCache; // unordered map hashes the string
};

