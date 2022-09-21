#pragma once
#include <gl/glew.h>
#include "Shaders/Shader.h"
#include "..\Interfaces/IApplication.h"
class TriangleExample : public IApplication
{
public:
	void PreLoopInit() override;
	void EventLoop() override;
	void Draw() override;
	void Tick() override;
	GLuint vao;
	Shader MyShader;
};

