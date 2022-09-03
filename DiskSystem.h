#pragma once
#include <Windows.h>
#include <memory>
#include <vector>
#include <filesystem>
#include <string>
#include <unordered_map>
#include <mutex>
#include <chrono>
#include <thread>

constexpr long long GARBAGE_COLLECTOR_THREAD_WAIT = 60;
constexpr long long STALE_FILE_SECONDS = 60;
constexpr long long STALE_FILE_NANOSECONDS = STALE_FILE_SECONDS * 1000000000;


struct DiskFile
{
	
	std::filesystem::path Path;
	size_t Size;

	std::vector<unsigned char> GetData();

	friend class DiskSystem;
private:
	[[nodiscard]]
	bool IsStale() const;
	std::chrono::time_point<std::chrono::steady_clock> Timestamp;
	long long GcNanoseconds = STALE_FILE_NANOSECONDS;
	std::mutex Mut;
	std::vector<BYTE> Data;
};

class DiskSystem
{
public:
	DiskSystem();
	DiskSystem(const std::string& rootPath);
	~DiskSystem();
	std::shared_ptr<DiskFile> GetFileCached(const std::string &path);
	std::vector<std::string> EnumDirectory(const std::string& path, bool bRecursive);
	std::vector<std::string> EnumDirectory(const std::string& path, const std::string& extension, bool bRecursive);
	void SetRootDirectory(const std::string& rootPath);
	void SetUseRootDirectory(bool bUseRootDir);
private:
	void GarbageCollect();
	std::thread GcThread;
	std::mutex CacheMut;
	std::mutex GcMut;
	bool bUseRootDirectory;
	bool bDestroyed;
	std::filesystem::path Root;
	std::unordered_map <std::string, std::shared_ptr<DiskFile>> FileCache; // unordered map hashes the string
};

