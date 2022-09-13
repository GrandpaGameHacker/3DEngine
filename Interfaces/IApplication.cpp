#include "IApplication.h"
#include "..\Logger.h"
#include "gl\glew.h"


IApplication::IApplication() = default;

IApplication::~IApplication()
{
	SDL_GL_DeleteContext(&GLContext);
	SDL_DestroyWindow(Window);
	SDL_Quit();
}

bool IApplication::Initialize(const char* appName, SDL_Rect rect,const UInt32 sdlFlags)
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

UInt32 IApplication::GetFlags() const
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

void IApplication::Start()
{
	PreLoopInit();
	while(bIsRunning)
	{
		EventLoop();
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