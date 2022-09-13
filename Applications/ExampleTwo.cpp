#include "ExampleTwo.h"

void ExampleTwo::PreLoopInit()
{
	Initialize("Example App 2", { SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800,600 }, 0);
}

void ExampleTwo::Draw()
{

}

void ExampleTwo::EventLoop()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
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

void ExampleTwo::Tick()
{

}
