#include "TriangleExample.h"
#include "../Logger.h"
#include "../Utilities/Disk/DiskSystem.h"
void TriangleExample::PreLoopInit()
{
	Initialize("TriangleExample", { SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600 }, NULL);

	const float points[] = {
		0.0f,  0.0f,  0.0f,
		+1.0f, +1.0f,  0.0f,
		-1.0f, +1.0f,  0.0f,

		0.0f,  0.0f,  0.0f,
		-1.0f, -1.0f,  0.0f,
		+1.0f, -1.0f,  0.0f
	};

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(float), points, GL_STATIC_DRAW);

	vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	DiskSystem Disk = DiskSystem();
	auto vShader = Disk.GetFile("/Applications/Shaders/TriangleExampleVertex.vert", true);
	auto fShader = Disk.GetFile("/Applications/Shaders/TriangleExampleFragment.frag", true);

	if (!vShader || !fShader)
	{
		Logger::LogDebug(" TriangleExample::Draw()", "Error, Shaders not loaded correctly");
		return;
	}

	auto vertex_shader = vShader->GetTextData().c_str();
	auto fragment_shader = fShader->GetTextData().c_str();

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertex_shader, nullptr);
	glCompileShader(vertexShader);

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragment_shader, nullptr);
	glCompileShader(fragmentShader);

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
}

void TriangleExample::EventLoop()
{
	SDL_Event event;
	while(SDL_PollEvent(&event))
	{
		switch(event.type)
		{
		case SDL_QUIT:
			bIsRunning = false;
			break;
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE)
				bIsRunning = false;
			break;
		default:
			break;
		}
	}
}

void TriangleExample::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(shaderProgram);
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	SDL_GL_SwapWindow(Window);
}

void TriangleExample::Tick()
{
}
