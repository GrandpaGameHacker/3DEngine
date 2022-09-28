#pragma once
#include <gl/glew.h>
#include <string>
#include <unordered_map>
class Texture
{
public:
	Texture() = default;
	Texture(GLenum target, const std::string& path);
	Texture(GLenum target);
	bool Load();
	bool Load(unsigned char* data, unsigned int size);
	void Bind(GLenum TextureUnit);
	void SetWrapMode(GLenum S, GLenum T);
	void SetFilterMode(GLenum MAG, GLenum MIN);
	void GetSize(int& Width, int& Height);
	void GetName(std::string& Name);
	void GetPath(std::string& Path);
	void LoadInternal(void* textureData);
	std::string FilePath;
	std::string FileName;
	GLenum Target;
	GLuint Object;
	GLenum WrapS, WrapT, Mag, Min;
	int Width;
	int Height;
	int BPP;
};

Texture LoadCachedTexture(GLenum target, const std::string& path);