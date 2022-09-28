#include "Utilities/Texture/Texture2D.h"
#include "Utilities/stb_image.h"
#include "Utilities/Disk/DiskSystem.h"
#include "Utilities/Logger.h"

GLuint Texture2D::GetTexture()
{
	return Texture;
}

Texture2D::Texture2D() :
	Texture(NULL),
	Size(NULL),
	Channels(NULL),
	ExternalSize(NULL),
	RealSize(NULL),
	UVCoords(NULL),
	PixelData(nullptr)
{
}

Texture2D::Texture2D(const Texture2D& copy)
	:
	Texture(copy.Texture),
	Size(copy.Size),
	Channels(copy.Channels),
	ExternalSize(copy.ExternalSize),
	RealSize(copy.RealSize),
	UVCoords(copy.UVCoords),
	PixelData(copy.PixelData),
	File(copy.File)
{

}

Texture2D::Texture2D(const std::string& textureFile) :
	Texture(0),
	Size(0),
	Channels(0),
	ExternalSize(1.0f),
	RealSize(0.0f),
	UVCoords(1.0f)
{
	File = textureFile;
	glGenTextures(1, &Texture);
	glBindTexture(GL_TEXTURE_2D, Texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height;
	PixelData = stbi_load(File.c_str(), &width, &height, &Channels, NULL);
	Size = { width, height };
	RealSize = { width, width };
	if (PixelData)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, PixelData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		Logger::LogDebug("Texture2D::Texture2D()", "Error, Example texture " + textureFile + "failed to load!");
	}
}

unsigned int Texture2D::GetWidth() const
{
	return Size.x;
}

unsigned int Texture2D::GetHeight() const
{
	return Size.y;
}

unsigned Texture2D::GetChannels()
{
	return Channels;
}

glm::ivec2 Texture2D::GetSize() const
{
	return Size;
}

glm::vec4 Texture2D::GetUV()
{
	return UVCoords;
}

void Texture2D::SetUV(glm::vec4 coords)
{
	UVCoords = coords;
}

void Texture2D::SetRenderSize(glm::ivec2 size)
{
	ExternalSize = size;
}

std::string Texture2D::GetFile()
{
	return File;
}

unsigned char* Texture2D::GetPixels()
{
	return PixelData;
}

void Texture2D::Bind()
{
	glBindTexture(GL_TEXTURE_2D, Texture);
}