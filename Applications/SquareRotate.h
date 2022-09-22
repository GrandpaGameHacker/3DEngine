#pragma once
#include <gl/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shaders/Shader.h"
#include "IApplication.h"

class SquareRotate :
	public IApplication
{
public:
	~SquareRotate() override;
private:
	void PreLoopInit() override;
	void Tick() override;
	void Draw() override;
	GLuint vbo = 0, ebo = 0;
	GLuint vao = 0;
	GLuint Texture = 0;
	Shader MyShader;
};