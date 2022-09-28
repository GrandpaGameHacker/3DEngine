#include "Applications/TextureExample.h"
#include "Utilities/stb_image.h"
#include "Utilities/Logger.h"
#include <chrono>
void TextureExample::PreLoopInit()
{
	Initialize("Example App Textures", { SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800,600 }, 0);

	MyShader.SetVertexShader("/Applications/Shaders/TextureExample.vs");
	MyShader.SetFragmentShader("/Applications/Shaders/TextureExample.fs");
	MyShader.Compile();

	MySecondShader.SetVertexShader("/Applications/Shaders/TextureExample2.vs");
	MySecondShader.SetFragmentShader("/Applications/Shaders/TextureExample2.fs");
	MySecondShader.Compile();

	float vertices[] = {
		// positions          // colors           // texture coordinates
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
	};

	int indices[] = {
		1,2,3,
		0,1,3,
	};

	GLuint vbo, ebo;

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)3);
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	int width, height, nrChannels;

	glGenTextures(1, &Texture);
	glBindTexture(GL_TEXTURE_2D, Texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load("Logo_1024x1024.png", &width, &height, &nrChannels, 0);
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
}

void TextureExample::Tick()
{
	if (UseCustomShader)
	{
		GLint location = glGetUniformLocation(CurrentShader->Get(), "variance");
		typedef std::chrono::high_resolution_clock Time;
		auto t0 = Time::now();
		float fs = t0.time_since_epoch().count() / 10000;
		glUniform1f(location, sin(fs));
	}
}


void TextureExample::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindTexture(GL_TEXTURE_2D, Texture);
	CurrentShader->Use();
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	SDL_GL_SwapWindow(Window);
}

void TextureExample::EventLoop(SDL_Event* event)
{
	switch (event->type)
	{
	case SDL_QUIT:
		bIsRunning = false;
		break;
	case SDL_KEYDOWN:
		if (event->key.keysym.sym == SDLK_ESCAPE)
			bIsRunning = false;
		if (event->key.keysym.sym == SDLK_s)
		{
			CurrentShader = &MySecondShader;
			UseCustomShader = true;
		}
		if (event->key.keysym.sym == SDLK_a)
		{
			CurrentShader = &MyShader;
			UseCustomShader = false;
		}
		break;
	default:
		break;
	}
}