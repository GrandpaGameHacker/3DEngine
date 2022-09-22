#pragma once
#include <gl/glew.h>

#include "Shaders/Shader.h"
#include "../Interfaces/IApplication.h"
class First3DExample :
	public IApplication
{
	void PreLoopInit() override;
	void Tick() override;
	void Draw() override;
	GLuint vao = 0;
	GLuint Texture = 0;
	Shader MyShader;
};