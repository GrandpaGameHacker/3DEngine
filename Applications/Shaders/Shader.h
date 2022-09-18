#pragma once
#include <gl/glew.h>
#include "../../Utilities/Disk/DiskSystem.h"
class Shader
{
public:
	Shader();
	Shader(std::string vertexShaderFile, std::string fragmentShaderFile);

	void SetVertexShaderFile(std::string vertexShaderFile);
	void SetFragmentShaderFile(std::string fragmentShaderFile);

	void SetVertexShader(const char* vertexShader);
	void SetFragmentShader(const char* vertexShader);

	GLuint Compile();
	GLuint GetShaderProgram();

private:
	bool isCompiled;
	std::string VertexShaderFile;
	std::string FragmentShaderFile;
	std::string VertexShader;
	std::string FragmentShader;
	GLuint ShaderProgram;
};
