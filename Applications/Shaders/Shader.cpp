#include "Shader.h"
#include "../../Logger.h"
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

void Shader::SetVertexShaderFile(std::string vertexShaderFile)
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

void Shader::SetFragmentShaderFile(std::string fragmentShaderFile)
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

void Shader::SetVertexShader(const char* vertexShader)
{
	VertexShader = vertexShader;
}

void Shader::SetFragmentShader(const char* fragmentShader)
{
	FragmentShader = fragmentShader;
}

GLuint Shader::Compile()
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
		return 0;
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
		return 0;
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
		return 0;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	isCompiled = true;
	return ShaderProgram;
}

GLuint Shader::GetShaderProgram()
{
	if(isCompiled)
	{
		return ShaderProgram;
	}
	else
	{
		Logger::LogDebug("Shader::GetShaderProgram()", "Error! Shader not compiled!");
	}
	return 0;
}
