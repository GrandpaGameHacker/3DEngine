#define SDL_MAIN_HANDLED
#include "Applications/SquareRotate.h"
#include "Logger.h"
int main(int argc, char** argv)
{
	SDL_SetMainReady();
	Logger::SetupLogging(Logger::LoggingLevel::All);
	SquareRotate app;
	app.Start();
	return 0;
}