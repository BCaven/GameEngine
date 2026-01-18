#include "SDL_Manager.h"
#include "Shape.h"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>



int main(int argc, char** argv) 
{
	spdlog::set_level(spdlog::level::info);
	auto logger = spdlog::stdout_color_mt("main");


	SDL_Manager& sdl = SDL_Manager::sdl();

	sdl.spawnWindow("Test Window 1", 500, 500, false);
	sdl.spawnWindow("Test Window 2", 500, 500, false);

	// test building shapes:
	logger->info("Creating shape from static data");
	Shape StaticShape(1, { 0.0, 0.0, 0.0, 0.5, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0 });
	logger->info("Creating shape from file");
	auto ShapeFromFile = Shape::fromFile("suzanne.bcf");

	if (!ShapeFromFile)
	{
		logger->warn("Failed to read shape from file!");
	}


	SDL_Event e;
	bool quit = false;
	while (!quit)
	{
		while (SDL_PollEvent(&e) != 0)
		{
			switch (e.type)
			{
			case SDL_EVENT_QUIT:
				quit = true;
				break;
			case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
				sdl.closeWindow(e.window.windowID);
				break;

			case SDL_EVENT_WINDOW_RESIZED:
				break;

			}
		}

		if (debugging::checkOpenGLErrors())
		{
			quit = true;
		}

		sdl.updateWindows();
	}
	return 0;
}