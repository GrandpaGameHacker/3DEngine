#include "TextureExample.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../Utilities/stb_image.h"
#include <gl\glew.h>
#include "../Logger.h"
void TextureExample::PreLoopInit()
{
	Initialize("Example App 2", { SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800,600 }, 0);

	float texCoords[] = {
	0.0f, 0.0f,  // lower-left  
	1.0f, 0.0f,  // lower-right
	0.5f, 1.0f   // top-center
	};
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	int width, height, nrChannels;

	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	unsigned char* data = stbi_load("test.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		Logger::LogDebug("TextureExample::PreLoopInit()", "Error, Example texture 'test.jpg' failed to load!");
		bIsRunning = false;
	}
	stbi_image_free(data);

	float vertices[] = {
		// positions          // colors           // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
	};

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

}

void TextureExample::Draw()
{

}

void TextureExample::EventLoop()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			bIsRunning = false;
			break;
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE)
				bIsRunning = false;
			break;
		default:
			break;
		}
	}
}

void TextureExample::Tick()
{

}
