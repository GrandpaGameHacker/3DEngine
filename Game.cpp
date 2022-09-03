#define SDL_MAIN_HANDLED
#include "Applications/TriangleExample.h"
#include "Applications/TriangleExampleTwo.h"
#include "DiskSystem.h"
#include "Logger.h"
int main(int argc, char** argv)
{
	SDL_SetMainReady();
	Logger::SetupLogging(Logger::LoggingLevel::All);
	DiskSystem myDisk = DiskSystem("C:/Users/twist/Desktop/");
	{
		auto myFile = myDisk.GetFile("shows.txt");
	}
	Sleep(60010);
	myDisk.GetFile("shows.txt");
	TriangleExampleTwo app;
	app.Init();
	app.Loop();
	return 0;
}