#pragma once
#include <gl/glew.h>
#include "Shaders/Shader.h"
#include "../Interfaces/IApplication.h"
class SquareExample : public IApplication
{
public:
	void PreLoopInit() override;
	void EventLoop(SDL_Event* event) override;
	void Draw() override;
	void Tick() override;
	GLuint vao;
	Shader MyShader;
};