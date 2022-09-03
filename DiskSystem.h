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

struct DiskFile
{
	
	std::filesystem::path Path;
	size_t Size;

	std::vector<unsigned char> GetData();

	friend class DiskSystem;
private:
	bool IsStale() const;
	std::chrono::time_point<std::chrono::steady_clock> Timestamp;
	long long GcNanoseconds = 60000000000;
	std::mutex Mut;
	std::vector<BYTE> Data;
};

class DiskSystem
{
public:
	DiskSystem();
	DiskSystem(const std::string& rootPath);
	~DiskSystem();
	std::shared_ptr<DiskFile> GetFile(const std::string &path);
	std::vector<std::string> EnumDirectory(const std::string& path);
	std::vector<std::string> EnumDirectoryEx(const std::string& path, const std::string& extension);
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

