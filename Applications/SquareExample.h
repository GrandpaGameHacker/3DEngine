#pragma once
#include <gl/glew.h>
#include "../Interfaces/IApplication.h"
class SquareExample : public IApplication
{
public:
	GLuint shaderProgram;
	GLuint vao;
	void PreLoopInit() override;
	void EventLoop() override;
	void Draw() override;
	void Tick() override;
};