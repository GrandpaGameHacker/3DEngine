#pragma once
#include <gl/glew.h>
#include "../../Utilities/Disk/DiskSystem.h"
class Shader
{
public:
	Shader();
	Shader(std::string vertexShaderFile, std::string fragmentShaderFile);
	~Shader();
	void SetVertexShader(std::string vertexShaderFile);
	void SetFragmentShader(std::string fragmentShaderFile);

	void Compile();
	void Recompile();
	GLuint Get();
	void Use();

private:
	bool isCompiled;
	std::string VertexShaderFile;
	std::string FragmentShaderFile;
	std::string VertexShader;
	std::string FragmentShader;
	GLuint ShaderProgram;
};

