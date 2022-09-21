#pragma once
#include <gl\glew.h>
#include "Shaders/Shader.h"
#include "..\Interfaces/IApplication.h"
class TextureExample : public IApplication
{
public:
	void PreLoopInit() override;
	void Draw() override;
	void EventLoop() override;
	void Tick() override;
	GLuint vao;
	GLuint Texture;
	Shader MyShader, MySecondShader;
	Shader* CurrentShader = &MyShader;
	bool UseCustomShader = false;
};

