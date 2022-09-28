#include "Applications/First3DExample.h"
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Utilities/stb_image.h"
#include "Utilities/Logger.h"

void First3DExample::PreLoopInit()
{
	Initialize("3D Camera Example", { SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800,600 }, 0);

	MyShader.SetVertexShader("/Assets/Shaders/3DExample.vs");
	MyShader.SetFragmentShader("/Assets/Shaders/3DExample.fs");
	MyShader.Compile();

	MyModel = Model(std::filesystem::current_path().string() + "/Assets/Models/backpack.obj");

	MyCamera.SetPosition({0.f, 0.f, 3.f});
	MyCamera.SetViewport(WRect.x, WRect.y, WRect.w, WRect.h);
	MyCamera.SetType(CameraType::PerspectiveFreecam);
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
	case SDL_MOUSEWHEEL:
		MyCamera.Roll2D(event->wheel.preciseY);
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
		break;
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
	MyModel.Draw(MyShader);
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
