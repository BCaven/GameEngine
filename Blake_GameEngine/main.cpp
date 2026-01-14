#include "SDL_Manager.h"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>


int main(int argc, char** argv) 
{
	spdlog::set_level(spdlog::level::info);
	auto logger = spdlog::stdout_color_mt("main");


	SDL_Manager& sdl = SDL_Manager::sdl();

	sdl.spawnWindow("Test Window 1", 500, 500, false);
	sdl.spawnWindow("Test Window 2", 500, 500, false);

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

		GLenum err;
		while ((err = glGetError()) != GL_NO_ERROR)
		{
			logger->error("OpenGL errors!");
			quit = true;
			break;
		}

		sdl.updateWindows();
	}
	return 0;
}