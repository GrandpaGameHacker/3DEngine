#include "First3DExample.h"
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../Utilities/stb_image.h"
#include "../Logger.h"
void First3DExample::PreLoopInit()
{
	Initialize("Example App Textures", { SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800,600 }, 0);

	MyShader.SetVertexShader("/Applications/Shaders/3DExample.vert");
	MyShader.SetFragmentShader("/Applications/Shaders/3DExample.frag");
	MyShader.Compile();

	float vertices[] = {
		// positions          // colors           // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
	};

	int indices[] = {
		1,2,3,
		0,1,3,
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

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)3);
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	Texture = Texture2D("test.jpg");
}

void First3DExample::Tick()
{

}

void First3DExample::Draw()
{

	MyShader.Use();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);
	glm::vec2 screen = GetScreenSize();

	model = glm::rotate(model, GetDeltaTime(), { 0.0f, 1.0f, 0.0f });
	view = glm::translate(view, { 0.0f, 0.0f, -2.0f });
	projection = glm::perspective(glm::radians(45.f), screen.x / screen.y, 0.1f, 100.f);

	MyShader.Set("projection", projection);
	MyShader.Set("view", view);
	MyShader.Set("model", model);

	Texture.Bind();

	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	SDL_GL_SwapWindow(Window);
}