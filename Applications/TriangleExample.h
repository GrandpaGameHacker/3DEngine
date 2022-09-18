#pragma once
#include <gl/glew.h>
#include "..\Interfaces/IApplication.h"
class TriangleExample : public IApplication
{
public:
	GLuint shaderProgram;
	GLuint vao = 0;
	void PreLoopInit() override;
	void EventLoop() override;
	void Draw() override;
	void Tick() override;
};

