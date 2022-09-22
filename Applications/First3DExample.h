#pragma once
#include <gl/glew.h>
#include "../Textures/Texture2D.h"
#include "Shaders/Shader.h"
#include "IApplication.h"
class First3DExample :
	public IApplication
{
	void PreLoopInit() override;
	void Tick() override;
	void Draw() override;
	GLuint vao = 0;
	Texture2D Texture;
	Shader MyShader;
};