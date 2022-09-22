#define SDL_MAIN_HANDLED
#include "Applications/First3DExample.h"
#include "Logger.h"
int main(int argc, char** argv)
{
	SDL_SetMainReady();
	Logger::SetupLogging(Logger::LoggingLevel::All);
	First3DExample app;
	app.Start();
	return 0;
}