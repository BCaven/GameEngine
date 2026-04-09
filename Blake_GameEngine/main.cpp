#include "SDL_Manager.h"
#include "Engine.h"
#include "RenderEngine.h"
#include "GameObject.h"
#include "RotatingObject.h"
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
#include <torch/torch.h>

// this should be the only place stb image gets defined
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

using namespace std::chrono_literals;
int main(int argc, char** argv) 
{
	spdlog::set_level(spdlog::level::info);
	auto logger = spdlog::stdout_color_mt("main");

	/*
	link libtorch :)
	*/
	if (torch::cuda::is_available())
	{
		logger->info("cuda is available!");
	}
	else
	{
		logger->warn("cuda is not available :(");
	}

	SDL_Manager& sdl = SDL_Manager::sdl();

	sdl.spawnWindow("Test Window 1", 1000, 500, false);
	//sdl.spawnWindow("Test Window 2", 500, 500, false);

	KeyInputs& inputManager = KeyInputs::inputHandler(&sdl);
	

	Engine engine = Engine();
	//engine.SpawnObject<GameObject>("beholder.bcf");
	auto BillboardRef = engine.SpawnObject<BillboardObject>("Suzanne.bcf");
	auto CameraObject = engine.SpawnObject<Camera>("Cone.bcf");
	BillboardRef->SetParent(CameraObject);

	Quaternion rot_amount(glm::vec3(0, std::numbers::pi / 4, 0));

	engine.SpawnObject<RotatingObject>("Suzanne.bcf", glm::vec3(1, 0, 0), glm::vec3(2, 0, 0));
	engine.SpawnObject<RotatingObject>("Suzanne.bcf", glm::vec3(0, 1, 0), glm::vec3(4, 0, 0));
	engine.SpawnObject<RotatingObject>("Suzanne.bcf", glm::vec3(0, 0, 1), glm::vec3(6, 0, 0));

	
	RenderEngine renderEngine = RenderEngine();
	renderEngine.initialize("simple", engine.GetSceneGraph());
	renderEngine.SetRenderCamera(CameraObject);

	engine.initialize();


	double prevTime = Utility::getTimeSeconds();
	while (!KeyInputs::QUIT)
	{
		// keyinputs should be in a different thread
		inputManager.handleKeyInputs();

		double Delta = Utility::getTimeSeconds() - prevTime;
		prevTime = Utility::getTimeSeconds();

		renderEngine.RenderFrame(Delta);
		
		if (debugging::checkOpenGLErrors())
		{
			KeyInputs::QUIT = true;
		}
		
		std::this_thread::sleep_for(1ms);
		sdl.updateWindows();

	}

	return 0;
}