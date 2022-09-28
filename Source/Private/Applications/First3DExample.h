#pragma once
#include <gl/glew.h>
#include "Utilities/Texture/Texture.h"
#include "Utilities/Shader/Shader.h"
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
	Texture MyTexture;
	Shader MyShader;
	Camera MyCamera;
};