#define SDL_MAIN_HANDLED

#define STB_IMAGE_IMPLEMENTATION
#include "Utilities/stb_image.h"
#include "Applications/First3DExample.h"
#include "Logger.h"
int main(int argc, char** argv)
{
	SDL_SetMainReady();

	stbi_set_flip_vertically_on_load(true);
	stbi_set_flip_vertically_on_load_thread(true);

	Logger::SetupLogging(Logger::LoggingLevel::All);
	First3DExample app;
	app.Start();
	return 0;
}