#include "Utilities/Shader/Shader.h"
#include "Utilities/Logger.h"

Shader::Shader()
{
	VertexShaderFile = "";
	FragmentShaderFile = "";

	VertexShader = "";
	FragmentShader = "";

	isCompiled = false;
	ShaderProgram = 0;
}

Shader::Shader(std::string vertexShaderFile, std::string fragmentShaderFile)
{
	DiskSystem shaderDisk = DiskSystem();
	auto vshader = shaderDisk.GetFileAsync(vertexShaderFile, true);
	auto fshader = shaderDisk.GetFileAsync(fragmentShaderFile, true);
	VertexShaderFile = vertexShaderFile;
	FragmentShaderFile = fragmentShaderFile;
	VertexShader = vshader.get()->GetTextData();
	FragmentShader = fshader.get()->GetTextData();
	if(VertexShader.empty() || FragmentShader.empty())
	{
		Logger::LogDebug("Shader::Shader()", "Shader(s) do not exist! | " + VertexShaderFile + " | " + FragmentShaderFile);
		isCompiled = false;
		return;
	}
	Compile();
}

Shader::~Shader()
{
	if(isCompiled)
	{
		glDeleteProgram(ShaderProgram);
	}
}

void Shader::SetVertexShader(std::string vertexShaderFile)
{
	DiskSystem shaderDisk = DiskSystem();
	auto vshader = shaderDisk.GetFileAsync(vertexShaderFile, true);
	VertexShaderFile = vertexShaderFile;
	VertexShader = vshader.get()->GetTextData();
	if (VertexShader.empty())
	{
		Logger::LogDebug("Shader::Shader()", "Shader does not exist! | " + VertexShaderFile);
	}
}

void Shader::SetFragmentShader(std::string fragmentShaderFile)
{
	DiskSystem shaderDisk = DiskSystem();
	auto fshader = shaderDisk.GetFileAsync(fragmentShaderFile, true);
	FragmentShaderFile = fragmentShaderFile;
	FragmentShader = fshader.get()->GetTextData();
	if (VertexShader.empty())
	{
		Logger::LogDebug("Shader::Shader()", "Shader does not exist! | " + FragmentShaderFile);
	}
}

void Shader::Compile()
{
	auto vs = VertexShader.c_str();
	auto fs = FragmentShader.c_str();

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vs, nullptr);
	glCompileShader(vertexShader);

	GLint isVertexCompiled = 0;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isVertexCompiled);
	if(isVertexCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(vertexShader, maxLength, &maxLength, infoLog.data());
		glDeleteShader(vertexShader);
		std::string logInfo = infoLog.data();
		Logger::LogDebug("Shader::Compile() VertexError: ", logInfo);
		return;
	}

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fs, nullptr);
	glCompileShader(fragmentShader);

	GLint isFragmentCompiled = 0;
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isFragmentCompiled);
	if (isVertexCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, infoLog.data());
		glDeleteShader(fragmentShader);
		std::string logInfo = infoLog.data();
		Logger::LogDebug("Shader::Compile() FragmentError: ", logInfo);
		return;
	}

	ShaderProgram = glCreateProgram();
	glAttachShader(ShaderProgram, vertexShader);
	glAttachShader(ShaderProgram, fragmentShader);
	glLinkProgram(ShaderProgram);
	GLint isLinked = 0;
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &isLinked);
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(ShaderProgram, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(ShaderProgram, maxLength, &maxLength, infoLog.data());
		glDeleteProgram(ShaderProgram);
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		std::string logInfo = infoLog.data();
		Logger::LogDebug("Shader::Compile() Link Error: ", logInfo);
		return;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	isCompiled = true;
	return;
}

void Shader::Recompile()
{
	if (isCompiled)
	{
		glDeleteProgram(ShaderProgram);
	}
	DiskSystem shaderDisk = DiskSystem();
	auto vshader = shaderDisk.GetFileAsync(VertexShaderFile, true);
	auto fshader = shaderDisk.GetFileAsync(FragmentShaderFile, true);
	VertexShader = vshader.get()->GetTextData();
	FragmentShader = fshader.get()->GetTextData();
	if (VertexShader.empty() || FragmentShader.empty())
	{
		Logger::LogDebug("Shader::Shader()", "Shader(s) do not exist! | " + VertexShaderFile + " | " + FragmentShaderFile);
		isCompiled = false;
		return;
	}
	Compile();
}

GLuint Shader::Get()
{
	if(isCompiled)
	{
		return ShaderProgram;
	}
	else
	{
		Logger::LogDebug("Shader::Get()", "Error! Shader not compiled!");
	}
	return 0;
}

void Shader::Use()
{
	if(isCompiled)
	{
		glUseProgram(ShaderProgram);
	}
}

void Shader::Set(std::string name, bool value)
{
	glUniform1i(glGetUniformLocation(ShaderProgram, name.c_str()), value);
}

void Shader::Set(std::string name, int value)
{
	glUniform1i(glGetUniformLocation(ShaderProgram, name.c_str()), value);
}

void Shader::Set(std::string name, unsigned int value)
{
	glUniform1i(glGetUniformLocation(ShaderProgram, name.c_str()), value);
}

void Shader::Set(std::string name, float value)
{
	glUniform1f(glGetUniformLocation(ShaderProgram, name.c_str()), value);
}

void Shader::Set(std::string name, glm::vec2 &value)
{
	glUniform2fv(glGetUniformLocation(ShaderProgram, name.c_str()), 1, &value[0]);
}

void Shader::Set(std::string name, float x, float y)
{
	glUniform2f(glGetUniformLocation(ShaderProgram, name.c_str()), x, y);
}

void Shader::Set(std::string name, glm::vec3 &value)
{
	glUniform3fv(glGetUniformLocation(ShaderProgram, name.c_str()), 1, &value[0]);
}

void Shader::Set(std::string name, float x, float y, float z)
{
	glUniform3f(glGetUniformLocation(ShaderProgram, name.c_str()), x, y, z);
}

void Shader::Set(std::string name, glm::vec4 &value)
{
	glUniform4fv(glGetUniformLocation(ShaderProgram, name.c_str()), 1, &value[0]);
}

void Shader::Set(std::string name, float x, float y, float z, float w)
{
	glUniform4f(glGetUniformLocation(ShaderProgram, name.c_str()),x ,y ,z ,w);
}

void Shader::Set(std::string name, glm::mat2 &value)
{
	glUniformMatrix2fv(glGetUniformLocation(ShaderProgram, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

void Shader::Set(std::string name, glm::mat3 &value)
{
	glUniformMatrix3fv(glGetUniformLocation(ShaderProgram, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

void Shader::Set(std::string name, glm::mat4 &value)
{
	glUniformMatrix4fv(glGetUniformLocation(ShaderProgram, name.c_str()), 1, GL_FALSE, &value[0][0]);
}
