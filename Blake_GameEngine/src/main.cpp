#include "SDL_Manager.h"
#include "Engine.h"
#include "DeferredRenderEngine.h"
#include "RenderEngineForward.h"
#include "GameObject.h"
#include "RotatingObject.h"
#include "SceneGraph.h"
#include "BillboardObject.h"
#include "Texture.h"
//#include "SceneGraphBVH_cpu.h"

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
	debugging::initGLDebug();
	KeyInputs& inputManager = KeyInputs::inputHandler(&sdl);
	
	// instancing :)
	logger->info("Loading shader the first time:");
	auto InstancedTexture = std::make_shared<Texture>();
	auto PinkTexture = std::make_shared<Texture>("assets/pink.jpg");

	auto InstancedSuzanne = Shape::fromFile("assets/suzanne.buvf");
	auto InstancedCube = Shape::fromFile("assets/cube.buvf");
	auto BlenderSuzanne = Shape::fromFile("assets/suzanne.buvf");
	InstancedCube->setTexture(InstancedTexture);
	InstancedSuzanne->setTexture(PinkTexture);
	BlenderSuzanne->setTexture(InstancedTexture);
	logger->info("Scene graph:");
	auto sceneGraph = std::make_shared<SceneGraph>();

	logger->info("Engine:");
	Engine engine = Engine(sceneGraph);
	
	auto CameraObject = engine.SpawnObject<Camera>("assets/Cone.bcf");
	auto SuzanneObject = engine.SpawnObject<GameObject>(InstancedSuzanne, glm::vec3(-1.5, 0, 0), glm::vec3(1), glm::vec3(0));
	auto CubeObject = engine.SpawnObject<GameObject>(InstancedCube, glm::vec3(1.5, 0, 0), glm::vec3(1), glm::vec3(0));
	auto SpinningSuzanne = engine.SpawnObject<RotatingObject>(BlenderSuzanne, glm::vec3(1, 0, 0), glm::vec3(0, 2, 0));
	sceneGraph->setActiveCamera(CameraObject);

	auto renderEngine = std::make_shared<DeferredRenderEngine>();
	renderEngine->initialize(engine.GetSceneGraph());
	renderEngine->SetRenderCamera(CameraObject);

	engine.initialize();


	double prevTime = Utility::getTimeSeconds();
	while (!KeyInputs::QUIT)
	{
		// keyinputs should be in a different thread
		inputManager.handleKeyInputs();

		double Delta = Utility::getTimeSeconds() - prevTime;
		prevTime = Utility::getTimeSeconds();

		renderEngine->RenderFrame(Delta);
		
		if (debugging::checkOpenGLErrors("Main Render Loop", 90))
		{
			KeyInputs::QUIT = true;
		}
		
		std::this_thread::sleep_for(1ms);
		sdl.updateWindows();

	}

	return 0;
}