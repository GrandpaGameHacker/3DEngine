#pragma once
#include <gl/glew.h>
#include "ITexture.h"

class Texture2D :
    public ITexture
{
public:
    Texture2D();
    Texture2D(const Texture2D& copy);
    Texture2D(const std::string& textureFile);

    GLuint GetTexture() override;
    unsigned int GetWidth() const override;
    unsigned int GetHeight() const override;
	unsigned GetChannels() override;
    glm::ivec2 GetSize() const override;

    // idk how to do UV stuff yet really, this data type could be wrong!
    glm::vec4 GetUV() override;

    void SetUV(glm::vec4 coords) override;
    void SetRenderSize(glm::ivec2 size) override;
    std::string GetFile() override;
    unsigned char* GetPixels() override;
    void Bind() override;

private:
    GLuint Texture;
    glm::ivec2 Size;
    int Channels;
    glm::vec2 ExternalSize;
    glm::vec2 RealSize;
    glm::vec4 UVCoords;
	unsigned char* PixelData;
    std::string File;

};

