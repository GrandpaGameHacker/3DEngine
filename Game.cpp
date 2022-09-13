#define SDL_MAIN_HANDLED
#include "Applications/TriangleExample.h"
#include "Applications/ExampleTwo.h"
#include "Utilities/Disk/DiskSystem.h"
#include "Logger.h"
int main(int argc, char** argv)
{
	SDL_SetMainReady();
	Logger::SetupLogging(Logger::LoggingLevel::All);
	TriangleExample app;
	app.Start();
	return 0;
}