#include "Applications\IApplication.h"
#include "Utilities\Logger.h"
#include "gl\glew.h"
#include <chrono>

IApplication::IApplication() = default;

IApplication::~IApplication()
{
	SDL_GL_DeleteContext(&GLContext);
	SDL_DestroyWindow(Window);
	SDL_Quit();
}

// Must call in PreLoopInit overload!
bool IApplication::Initialize(const char* appName, SDL_Rect rect,const unsigned int sdlFlags)
{
	WindowsDPIScaling();
	if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		Logger::LogDebug("IApplication::Initialize()", "SDL_Init(SDL_INIT_EVERYTHING) failed!");
		bIsWindowValid = false;
		bIsRunning = false;
		return false;
	}
	Logger::LogInfo("IApplication::Initialize()", "SDL_INIT_SUCESS");
	Window = SDL_CreateWindow(appName, rect.x, rect.y, rect.w, rect.h, sdlFlags | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI);
	if(!Window)
	{
		Logger::LogDebug("IApplication::Initialize()", "SDL_CreateWindow failed");
		bIsWindowValid = false;
		bIsRunning = false;
		return false;
	}
	GLContext = SDL_GL_CreateContext(Window);
	if (!GLContext)
	{
		Logger::LogDebug("IApplication::Initialize()", "SDL_GL_CreateContext failed!");
		bIsWindowValid = false;
		bIsRunning = false;
		return false;
	}
	glewInit();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_DST_ALPHA);
	glDepthFunc(GL_LESS);

	const std::string renderer = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
	const std::string version = reinterpret_cast<const char*>(glGetString(GL_VERSION));
	Logger::LogInfo("Renderer -", renderer);
	Logger::LogInfo("OpenGL Version -", version);
	WRect = { 0,0, rect.w, rect.h };
	return true;
}

bool IApplication::ShowWindow(const bool bShow)
{
	if(bIsWindowValid && bIsRunning)
	{
		if(bShow)
		{
			SDL_ShowWindow(Window);
		}
		else
		{
			SDL_HideWindow(Window);
		}
		return true;
	}

	return false;
}

SDL_Window* IApplication::GetSDLWindow() const
{
	return Window;
}

SDL_GLContext IApplication::GetGLContext() const
{
	return GLContext;
}

bool IApplication::IsRunning() const
{
	return bIsRunning;
}

bool IApplication::IsValid() const
{
	return bIsWindowValid;
}

unsigned int IApplication::GetFlags() const
{
	return flags;
}

std::string IApplication::GetName() const
{
	return Name;
}

glm::ivec2 IApplication::GetScreenSize() const
{
	int Width, Height;
	SDL_GetWindowSize(Window, &Width, &Height);
	return glm::ivec2({Width,Height});
}

glm::ivec2 IApplication::GetDrawableSize() const
{
	int Width, Height;
	SDL_GL_GetDrawableSize(Window, &Width, &Height);
	return glm::ivec2({ Width,Height });
}

float IApplication::GetDeltaTime()
{
	typedef std::chrono::high_resolution_clock Time;
	static long long started = Time::now().time_since_epoch().count();
	long long now = Time::now().time_since_epoch().count();
	return static_cast<float>(now - started) / 1000000000.f;
}

// rewrite when we create generic rendering classes
void IApplication::ResizeHandler(SDL_Event* event)
{
	if(event->window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
	{
		auto screen = GetDrawableSize();
		int x, y;
		SDL_GetWindowPosition(Window, &x, &y);
		WRect = { x,y,screen.x, screen.y };
		glViewport(x, y, screen.x, screen.y);
	}
}

// Default Event Loop

void IApplication::EventLoop(SDL_Event* event)
{
	switch (event->type)
	{
	case SDL_QUIT:
		bIsRunning = false;
		break;
	case SDL_KEYDOWN:
		if (event->key.keysym.sym == SDLK_ESCAPE)
			bIsRunning = false;
		break;
	default:
		break;
	}
}

void IApplication::Start()
{
	PreLoopInit();
	while(bIsRunning)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			EventLoop(&event);
		}
		Tick();
		Draw();
	}
}

void IApplication::WindowsDPIScaling()
{
	typedef enum PROCESS_DPI_AWARENESS
	{
		PROCESS_DPI_UNAWARE = 0,
		PROCESS_SYSTEM_DPI_AWARE = 1,
		PROCESS_PER_MONITOR_DPI_AWARE = 2
	} PROCESS_DPI_AWARENESS;

	void* userDLL;
	BOOL(WINAPI * SetProcessDPIAware)(void) = nullptr; // Vista and later
	void* shcoreDLL;
	HRESULT(WINAPI * SetProcessDpiAwareness)(PROCESS_DPI_AWARENESS dpiAwareness) = nullptr; // Windows 8.1 and later

	userDLL = SDL_LoadObject("USER32.DLL");
	if (userDLL) {
		SetProcessDPIAware = (BOOL(WINAPI*)(void)) SDL_LoadFunction(userDLL, "SetProcessDPIAware");
	}

	shcoreDLL = SDL_LoadObject("SHCORE.DLL");
	if (shcoreDLL) {
		SetProcessDpiAwareness = (HRESULT(WINAPI*)(PROCESS_DPI_AWARENESS)) SDL_LoadFunction(shcoreDLL, "SetProcessDpiAwareness");
	}

	if (SetProcessDpiAwareness) {
		/* Try Windows 8.1+ version */
		const HRESULT result = SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);
		SDL_Log("called SetProcessDpiAwareness: %d\n", (result == S_OK) ? 1 : 0);
	}
	else if (SetProcessDPIAware) {
		/* Try Vista - Windows 8 version.
		This has a constant scale factor for all monitors.
		*/
		const BOOL success = SetProcessDPIAware();
		SDL_Log("called SetProcessDPIAware: %d\n", static_cast<int>(success));
	}
}