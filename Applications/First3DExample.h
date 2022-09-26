#pragma once
#include <gl/glew.h>
#include "../Textures/Texture2D.h"
#include "Shaders/Shader.h"
#include "IApplication.h"
#include "../Utilities/Camera/Camera.h"

class First3DExample :
	public IApplication
{
	void PreLoopInit() override;
	void Tick() override;
	void EventLoop(SDL_Event* event) override;
	void Draw() override;
	void ResizeHandler(SDL_Event* event) override;
	GLuint vao = 0;
	Texture2D Texture;
	Shader MyShader;
	Camera MyCamera;
};