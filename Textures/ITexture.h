#pragma once
#include <glm/glm.hpp>
#include <memory>
#include <string>
class ITexture
{
public:
	ITexture() { };
	virtual ~ITexture() { };
	virtual unsigned int GetTexture() = 0;
	virtual unsigned int GetWidth() const = 0;
	virtual unsigned int GetHeight() const = 0;
	virtual unsigned int GetChannels() = 0;
	virtual glm::ivec2 GetSize() const = 0;
	virtual void SetUV(glm::vec4 coords) = 0;
	virtual glm::vec4 GetUV() = 0;
	virtual void SetRenderSize(glm::ivec2 size) = 0;
	virtual std::string GetFile() = 0;
	virtual unsigned char* GetPixels() = 0;
	virtual void Bind() = 0;
};

