#pragma once
#include <gl/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shaders/Shader.h"
#include "../Interfaces/IApplication.h"

class SquareRotate :
	public IApplication
{
	void PreLoopInit() override;
	void EventLoop() override;
	void Tick() override;
	void Draw() override;
	GLuint vao;
	GLuint Texture;
	Shader MyShader;
};