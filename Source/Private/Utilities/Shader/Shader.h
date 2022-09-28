#pragma once
#include <gl/glew.h>
#include <glm/glm.hpp>
#include "Utilities/Disk/DiskSystem.h"
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

	void Set(std::string name, bool value);
	void Set(std::string name, int value);
	void Set(std::string name, unsigned int value);
	void Set(std::string name, float value);
	void Set(std::string name, glm::vec2 &value);
	void Set(std::string name, float x, float y);
	void Set(std::string name, glm::vec3 &value);
	void Set(std::string name, float x, float y, float z);
	void Set(std::string name, glm::vec4 &value);
	void Set(std::string name, float x,float y,float z,float w);
	void Set(std::string name, glm::mat2 &value);
	void Set(std::string name, glm::mat3 &value);
	void Set(std::string name, glm::mat4 &value);


private:
	bool isCompiled;
	std::string VertexShaderFile;
	std::string FragmentShaderFile;
	std::string VertexShader;
	std::string FragmentShader;
	GLuint ShaderProgram;
};

