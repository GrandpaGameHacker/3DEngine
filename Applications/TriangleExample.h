#pragma once
#include <gl/glew.h>
#include "Shaders/Shader.h"
#include "..\Interfaces/IApplication.h"
class TriangleExample : public IApplication
{
public:
	Shader AppShader;
	GLuint vao = 0;
	void PreLoopInit() override;
	void EventLoop() override;
	void Draw() override;
	void Tick() override;
};

