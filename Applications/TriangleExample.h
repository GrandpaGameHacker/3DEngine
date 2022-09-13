#pragma once
#include <gl/glew.h>

#include "..\Interfaces/IApplication.h"
class TriangleExample : public IApplication
{
public:
	GLuint shaderProgram;
	GLuint vao = 0;
	const float points[9] = {
0.0f,  0.5f,  0.0f,
0.5f, -0.5f,  0.0f,
-0.5f, -0.5f,  0.0f
	};

	void PreLoopInit() override;
	void EventLoop() override;
	void Draw() override;
	void Tick() override;
};

