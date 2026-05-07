#include "SDL_Manager.h"
#include "Engine.h"
#include "RenderEngine.h"
#include "GameObject.h"
#include "RotatingObject.h"
//#include "SceneGraph.h"
#include "BillboardObject.h"
#include "SceneGraphBVH_cpu.h"

#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>
#include <cmath>
#include <thread>
#include <chrono>

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
	
	if (torch::cuda::is_available())
	{
		logger->info("cuda is available!");
	}
	else
	{
		logger->warn("cuda is not available :(");
	}
	*/
	SDL_Manager& sdl = SDL_Manager::sdl();

	sdl.spawnWindow("Test Window 1", 1000, 500, false);
	//sdl.spawnWindow("Test Window 2", 500, 500, false);

	KeyInputs& inputManager = KeyInputs::inputHandler(&sdl);
	
	// instancing :)
	auto InstancedShader = std::make_shared<Shader>("simple");
	auto InstancedSuzanne = Shape::fromFile("suzanne.buvf", InstancedShader);
	auto sceneGraph = std::make_shared<SceneGraphBVH_cpu>();
	Engine engine = Engine(sceneGraph);
	//engine.SpawnObject<GameObject>("beholder.bcf");
	//auto BillboardRef = engine.SpawnObject<BillboardObject>("cube.buvf");
	auto CameraObject = engine.SpawnObject<Camera>("Cone.bcf");
	//engine.SpawnObject<RotatingObject>(InstancedSuzanne, glm::vec3(1, 0, 0), glm::vec3(2, 0, 0));

	sceneGraph->setActiveCamera(CameraObject);
	//BillboardRef->SetParent(CameraObject);

	Quaternion rot_amount(glm::vec3(0, std::numbers::pi / 4, 0));
	
	for (int i = 0; i < 1; i+=1)
	{
		engine.SpawnObject<RotatingObject>(InstancedSuzanne, glm::vec3(1, 0, 0), glm::vec3(i, 0, 0));
	}
	
	RenderEngine renderEngine = RenderEngine();
	renderEngine.initialize(engine.GetSceneGraph());
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