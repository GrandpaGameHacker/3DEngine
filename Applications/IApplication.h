#pragma once
#include "SDL2/SDL.h"
#include "glm/glm.hpp"
#include <string>
class IApplication
{
	static void WindowsDPIScaling();
public:
	virtual ~IApplication();
	IApplication();

	virtual bool Initialize(const char* appName, SDL_Rect rect, const unsigned int sdlFlags);
	virtual bool ShowWindow(bool bShow);

	SDL_Window* GetSDLWindow() const;
	SDL_GLContext GetGLContext() const;
	bool IsRunning() const;
	bool IsValid() const;
	unsigned int GetFlags() const;
	std::string GetName() const;
	glm::ivec2 GetScreenSize() const;
	glm::ivec2 GetDrawableSize() const;
	float GetDeltaTime();

	virtual void ResizeHandler(SDL_Event* event);
	virtual void PreLoopInit() = 0;
	virtual void EventLoop(SDL_Event* event);
	virtual void Tick() = 0;
	virtual void Draw() = 0;
	virtual void Start();


protected:

	SDL_Window* Window = nullptr;
	SDL_GLContext GLContext = nullptr;

	const char* Name = nullptr;
	SDL_Rect WRect = {};
	unsigned int flags = 0;

	bool bIsRunning = true;
	bool bIsWindowValid = true;
};

