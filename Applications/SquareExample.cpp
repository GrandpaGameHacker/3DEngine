#include "SquareExample.h"


void SquareExample::PreLoopInit()
{
	Initialize("Square Example (EBO's)", { SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,800,800 }, NULL);
	const float vertices[] =
	{
		+0.5f, +0.5f, 0.0f, // top-right
		-0.5f, +0.5f, 0.0f, // bottom-right
		-0.5f, -0.5f, 0.0f, // bottom-left
		+0.5f, -0.5f, 0.0f, // top-left
	};

	const unsigned int indices[] = {
		1,2,3,
		0,1,3
	};

	GLuint vbo, ebo;
	glGenVertexArrays(1, &vao);

	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	MyShader.SetVertexShader("/Applications/Shaders/TriangleExampleVertex.vert");
	MyShader.SetFragmentShader("/Applications/Shaders/TriangleExampleFragment.frag");
	MyShader.Compile();

}

void SquareExample::EventLoop(SDL_Event* event)
{
	switch (event->type)
	{
	case SDL_QUIT:
		bIsRunning = false;
		break;
	case SDL_KEYDOWN:
		if (event->key.keysym.sym == SDLK_ESCAPE)
			bIsRunning = false;
		if (event->key.keysym.sym == SDLK_w)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		if (event->key.keysym.sym == SDLK_f)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		break;
	default:
		break;
	}
}

void SquareExample::Tick()
{
}

void SquareExample::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	MyShader.Use();
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	SDL_GL_SwapWindow(Window);
	glBindVertexArray(0);
}
