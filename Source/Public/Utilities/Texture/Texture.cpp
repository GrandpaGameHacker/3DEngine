#include "Utilities/Texture/Texture.h"
#include <filesystem>
#include "Utilities/Logger.h"
#include "stb_image.h"
Texture::Texture(GLenum target, const std::string& path)
{
	Target = target;
	FilePath = path;
	FileName = std::filesystem::path(path).stem().string();
	Width = 0;
	Height = 0;
	BPP = 0;
	Object = 0;
	WrapS = GL_REPEAT;
	WrapT = GL_REPEAT;
	Mag = GL_LINEAR;
	Min = GL_LINEAR;
}

Texture::Texture(GLenum target)
{
	Target = target;
	FilePath = "MEMORY_TEXTURE";
	FileName = "MEMORY_TEXTURE";
	Width = 0;
	Height = 0;
	BPP = 0;
	Object = 0;
}

bool Texture::Load()
{
	unsigned char* textureData = stbi_load(FilePath.c_str(), &Width, &Height, &BPP, NULL);
	if (!textureData)
	{
		Logger::LogDebug("Texture::Texture()", "Error -> " + FileName + "failed to load!");
		exit(0);
	}

	LoadInternal(textureData);
	stbi_image_free(textureData);
	Logger::LogInfo("Texture::Texture()", "Texture: " + FileName + " Loaded");


	return true;
}

bool Texture::Load(unsigned char* data, unsigned int size)
{
	if (FileName == "MEMORY_TEXTURE") {
		unsigned char* textureData = stbi_load_from_memory(data, size, &Width, &Height, &BPP, NULL);
		if (!textureData)
		{
			Logger::LogDebug("Texture::Texture()", "Error -> " + FileName + "failed to load!");
			exit(0);
		}

		Logger::LogInfo("Texture::Texture()", "Texture: Loaded from memory!");
		LoadInternal(textureData);
		stbi_image_free(textureData);
		return true;
	}
	else {
		Logger::LogDebug("Texture::Load()", "Attempting to load memory texture into file texture!");
		return false;
	}

}

void Texture::Bind(GLenum TextureUnit)
{
	glBindTextureUnit(TextureUnit, Object);
}

void Texture::SetWrapMode(GLenum S, GLenum T)
{
	glBindTexture(Target, Object);
	glTexParameteri(Target, GL_TEXTURE_WRAP_S, S);
	glTexParameteri(Target, GL_TEXTURE_WRAP_T, T);
	glBindTexture(Target, 0);
}

void Texture::SetFilterMode(GLenum MAG, GLenum MIN)
{
	glBindTexture(Target, Object);
	glTexParameteri(Target, GL_TEXTURE_MIN_FILTER, MIN);
	glTexParameteri(Target, GL_TEXTURE_MAG_FILTER, MAG);
	glBindTexture(Target, 0);
}

void Texture::GetSize(int& Width, int& Height)
{
	Width = this->Width;
	Height = this->Height;
}

void Texture::GetName(std::string& Name)
{
	Name = FileName;
}

void Texture::GetPath(std::string& Path)
{
	Path = FilePath;
}

void Texture::LoadInternal(void* textureData)
{
	glGenTextures(1, &Object);
	glBindTexture(Target, Object);
	switch (Target)
	{

	case GL_TEXTURE_3D:
		Logger::LogDebug("Texture::LoadInternal()", "GL_TEXTURE_3D Not Implemented");
		exit(0);
		break;

	case GL_TEXTURE_2D:
		switch (BPP)
		{
		case 1:
			glTexImage2D(Target, NULL, GL_RED, Width, Height, NULL, GL_RED, GL_UNSIGNED_BYTE, textureData);
			break;
		case 3:
			glTexImage2D(Target, NULL, GL_RGB, Width, Height, NULL, GL_RGB, GL_UNSIGNED_BYTE, textureData);
			break;
		case 4:
			glTexImage2D(Target, NULL, GL_RGBA, Width, Height, NULL, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
			break;
		default:
			Logger::LogDebug("Texture::LoadInternal()", "INVALID_BPP!");
			exit(0);
			break;
		}
		glGenerateMipmap(GL_TEXTURE_2D);
		break;

	case GL_TEXTURE_1D:
		Logger::LogDebug("Texture::Load()", "GL_TEXTURE_1D Not Implemented");
		exit(0);
		break;
	}

	glTexParameteri(Target, GL_TEXTURE_WRAP_S, WrapS);
	glTexParameteri(Target, GL_TEXTURE_WRAP_T, WrapT);
	glTexParameteri(Target, GL_TEXTURE_MIN_FILTER, Min);
	glTexParameteri(Target, GL_TEXTURE_MAG_FILTER, Mag);
	glBindTexture(Target, 0);
}

Texture LoadCachedTexture(GLenum target, const std::string& path)
{
	static std::unordered_map<std::string, Texture> TextureCache;
	if (!TextureCache.empty())
	{
		const auto cachedFile = TextureCache.find(path);
		if(cachedFile != TextureCache.end())
			return cachedFile->second;
	}
	auto tex = Texture(target, path);
	tex.Load();
	TextureCache.try_emplace(path, tex);
	return tex;
}
