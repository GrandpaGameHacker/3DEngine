#include "SquareRotate.h"
#include "../Utilities/stb_image.h"
#include "../Logger.h"

SquareRotate::~SquareRotate()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
}

void SquareRotate::PreLoopInit()
{
	Initialize("Rotating Square Example", { SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800,800 }, NULL);

	MyShader.SetVertexShader("/Applications/Shaders/SquareRotate.vert");
	MyShader.SetFragmentShader("/Applications/Shaders/SquareRotate.frag");
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

	float texCoords[] = {
	0.0f, 0.0f,  // lower-left  
	1.0f, 0.0f,  // lower-right
	0.5f, 1.0f   // top-center
	};

	float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	int width, height, nrChannels;

	glGenTextures(1, &Texture);
	glBindTexture(GL_TEXTURE_2D, Texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load("test.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		Logger::LogDebug("TextureExample::PreLoopInit()", "Error, Example texture 'test.jpg' failed to load!");
		bIsRunning = false;
	}
	stbi_image_free(data);
}

void SquareRotate::Tick()
{
}

void SquareRotate::Draw()
{
	glBindVertexArray(vao);
	MyShader.Use();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 trans = glm::mat4(1.0f); // translation matrix set to the identity matrix
	trans = glm::translate(trans, { 0.5f,-0.5f,0.f });
	trans = glm::rotate(trans, GetDeltaTime(), glm::vec3(0.0, 0.0, 1.0));
	GLint trans_loc = glGetUniformLocation(MyShader.Get(), "transform");
	glUniformMatrix4fv(trans_loc, 1, GL_FALSE, glm::value_ptr(trans));

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	trans = glm::mat4(1.0f);
	trans = glm::translate(trans, { -0.5f,0.5f,0.f });
	trans = glm::scale(trans, { sin(GetDeltaTime()),cos(GetDeltaTime()),1.0 });
	trans_loc = glGetUniformLocation(MyShader.Get(), "transform");
	glUniformMatrix4fv(trans_loc, 1, GL_FALSE, glm::value_ptr(trans));


	glBindTexture(GL_TEXTURE_2D, Texture);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	SDL_GL_SwapWindow(Window);
}
