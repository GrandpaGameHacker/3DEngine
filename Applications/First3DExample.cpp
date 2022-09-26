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

	Texture = Texture2D("Logo_1024x1024.png");
	MyCamera.SetPosition({0.f, 0.f, 3.f});
	MyCamera.SetViewport(WRect.x, WRect.y, WRect.w, WRect.h);
	MyCamera.SetType(CameraType::Perspective);
}

void First3DExample::Tick()
{

}

void First3DExample::EventLoop(SDL_Event* event)
{
	IApplication::EventLoop(event);
	switch(event->type)
	{
	case SDL_WINDOWEVENT:
		if (event->window.event != SDL_WINDOWEVENT_SIZE_CHANGED) break;
		ResizeHandler(event);
		break;
	case SDL_KEYDOWN:
		switch (event->key.keysym.sym)
		{
			case 'w':
				MyCamera.SetMovement(CameraMove::Forward, true);
			break;
			case 's':
				MyCamera.SetMovement(CameraMove::Back, true);
			break;
			case 'a':
				MyCamera.SetMovement(CameraMove::Left, true);
				break;
			case 'd':
				MyCamera.SetMovement(CameraMove::Right, true);
				break;
		}
			break;
	case SDL_KEYUP:
		switch (event->key.keysym.sym)
		{
		case 'w':
			MyCamera.SetMovement(CameraMove::Forward, false);
			break;
		case 's':
			MyCamera.SetMovement(CameraMove::Back, false);
			break;
		case 'a':
			MyCamera.SetMovement(CameraMove::Left, false);
			break;
		case 'd':
			MyCamera.SetMovement(CameraMove::Right, false);
			break;
		}
	case SDL_MOUSEMOTION:
		MyCamera.Rotate2D(event->motion.x , event->motion.y);
		break;
	case SDL_MOUSEBUTTONDOWN:
		MyCamera.SetPos(event->button.button, event->button.state, event->button.x, event->button.y);
		break;
	case SDL_MOUSEBUTTONUP:
		MyCamera.SetPos(event->button.button, event->button.state, event->button.x, event->button.y);
	default:
		break;
	}
}

void First3DExample::Draw()
{

	MyShader.Use();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);;
	glm::mat4 MVP;
	MyCamera.Update();
	MyCamera.GetMVP(MVP);
	MyShader.Set("MVP", MVP);
	Texture.Bind();

	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	SDL_GL_SwapWindow(Window);
}

void First3DExample::ResizeHandler(SDL_Event* event)
{
	auto screen = GetScreenSize();
	int x, y;
	SDL_GetWindowPosition(Window, &x, &y);
	WRect = { x,y,screen.x, screen.y };
	MyCamera.SetViewport(x, y, screen.x, screen.y);
}
