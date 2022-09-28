#include "Applications/TriangleExample.h"
#include "Utilities/Logger.h"
#include "Utilities/Disk/DiskSystem.h"
#include "Utilities/Shader/Shader.h"

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

	MyShader.SetVertexShader("/Applications/Shaders/TriangleExampleVertex.vert");
	MyShader.SetFragmentShader("/Applications/Shaders/TriangleExampleFragment.frag");
	MyShader.Compile();
}

void TriangleExample::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	MyShader.Use();
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	SDL_GL_SwapWindow(Window);
}

void TriangleExample::Tick()
{
}
