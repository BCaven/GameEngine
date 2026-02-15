#include "RenderEngine.h"

#include "SDL_Manager.h"
#include "Shape.h"
#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>
#include <cmath>

int main(int argc, char** argv) 
{
	spdlog::set_level(spdlog::level::info);
	auto logger = spdlog::stdout_color_mt("main");


	SDL_Manager& sdl = SDL_Manager::sdl();

	sdl.spawnWindow("Test Window 1", 500, 500, false);
	//sdl.spawnWindow("Test Window 2", 500, 500, false);

	// test building shapes:
	auto ShapeFromFile = Shape::fromFile("cube.bcf");
	if (!ShapeFromFile)
	{
		logger->warn("Failed to read shape from file");
		return -1;
	}
	
	RenderEngine renderEngine;
	renderEngine.initialize();
	renderEngine.addToRenderQueue(ShapeFromFile);


	SDL_Event e;
	bool quit = false;
	double frame_num = 0;
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

		// display mesh
		renderEngine.RenderFrame(frame_num);

		if (debugging::checkOpenGLErrors())
		{
			quit = true;
		}

		sdl.updateWindows();
		frame_num += 0.001;
	}
	return 0;
}