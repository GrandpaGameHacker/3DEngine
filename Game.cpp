#define SDL_MAIN_HANDLED
#include "Applications/TriangleExample.h"
#include "Applications/TextureExample.h"
#include "Applications/SquareExample.h"
#include "Logger.h"
int main(int argc, char** argv)
{
	SDL_SetMainReady();
	Logger::SetupLogging(Logger::LoggingLevel::All);
	SquareExample app;
	app.Start();
	return 0;
}