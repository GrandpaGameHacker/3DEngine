#pragma once
#include <gl/glew.h>
#include "Utilities/Model/Model.h"
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
	Model MyModel;
	Shader MyShader;
	Camera MyCamera;
};