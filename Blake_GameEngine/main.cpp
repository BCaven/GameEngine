#include "SDL_Manager.h"
#include "Engine.h"
#include "RenderEngine.h"
#include "GameObject.h"
#include "SceneGraph.h"
#include "BillboardObject.h"

#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>
#include <cmath>
#include <thread>
#include <chrono>

using namespace std::chrono_literals;
int main(int argc, char** argv) 
{
	const char* FORWARD = "w";

	spdlog::set_level(spdlog::level::info);
	auto logger = spdlog::stdout_color_mt("main");


	SDL_Manager& sdl = SDL_Manager::sdl();

	sdl.spawnWindow("Test Window 1", 500, 500, false);
	//sdl.spawnWindow("Test Window 2", 500, 500, false);
	

	Engine engine = Engine();
	engine.initialize();
	//engine.SpawnObject<GameObject>("beholder.bcf");
	engine.SpawnObject<BillboardObject>("Suzanne.bcf");
	engine.SpawnObject<GameObject>("ExtrudedCode.bcf");


	auto CameraObject = std::make_shared<Camera>();

	RenderEngine renderEngine = RenderEngine();
	renderEngine.initialize("simple", engine.GetSceneGraph());
	renderEngine.SetRenderCamera(CameraObject);

		

	SDL_Event e;
	bool quit = false;
	double prevTime = Utility::getTimeSeconds();
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

			case SDL_EVENT_KEY_DOWN:
				// TODO: actual input system

				double Delta = Utility::getTimeSeconds() - prevTime;
				if (e.key.key == SDLK_W)
				{
					//logger->info("Delta: {}", Delta);
					//renderEngine.camDist += Delta * 10;
				}
				if (e.key.key == SDLK_S)
				{
					//logger->info("Delta: {}", Delta);
					//renderEngine.camDist -= Delta * 10;
				}

			}
		}
		renderEngine.RenderFrame(Utility::getTimeSeconds());
		prevTime = Utility::getTimeSeconds();
		if (debugging::checkOpenGLErrors())
		{
			quit = true;
		}

		std::this_thread::sleep_for(1ms);
		sdl.updateWindows();

	}

	return 0;
}