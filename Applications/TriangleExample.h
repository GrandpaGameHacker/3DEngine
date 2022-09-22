#pragma once
#include <gl/glew.h>
#include "Shaders/Shader.h"
#include "..\Interfaces/IApplication.h"
class TriangleExample : public IApplication
{
public:
	void PreLoopInit() override;
	void Draw() override;
	void Tick() override;
private:
	GLuint vao = 0;
	Shader MyShader;
};

