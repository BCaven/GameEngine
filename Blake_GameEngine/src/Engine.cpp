#include "Engine.h"

Engine::Engine(std::shared_ptr<SceneGraph> sceneGraph_ptr)
{
	logger = spdlog::get("engine");
	if (!logger)
	{
		logger = spdlog::stdout_color_mt("engine");
	}
	sceneGraph = sceneGraph_ptr;

}

void Engine::initialize()
{
	// Initialization code here
	bRunning = true;


	// construct our scene graph + register game objects that tick
	// for now, just going to put them in an array
	for (auto obj : sceneGraph->GetAllObjects())
	{
		if (obj->CanTick())
		{
			TickableGameObjects.push_back(obj);
		}
	}


	// start main game loop thread
	mainGameLoopThread = std::thread(&Engine::MainGameLoop, this);
}

void Engine::MainGameLoop()
{

	// TODO: multi-threading race conditions :)

	logger->info("Starting main game loop thread.");
	const double MillisToSeconds = 1 / 1000.0;
	while (bRunning)
	{
		Uint64 deltaTimeMillis = Utility::getTimeMillis();
		Uint64 deltaMillis = deltaTimeMillis - PrevFrameTimeMillis;
		PrevFrameTimeMillis = deltaTimeMillis;
		double deltaSeconds = deltaMillis * MillisToSeconds;

		for (auto obj : TickableGameObjects)
		{
			obj->Tick(deltaSeconds); // Convert to seconds
		}

		// TODO: whatever else needs to happen each frame

		// TBD: Might be able to be removed later
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
	
}

void Engine::RegisterGameObject(std::shared_ptr<GameObject> obj)
{
	// TODO: make this something that gets triggered when the SceneGraph gets updated
	GameObjects.push_back(obj);
	if (obj->CanTick())
	{
		TickableGameObjects.push_back(obj);
	}
}

void Engine::Shutdown()
{
	if (!bRunning)
	{
		logger->info("Shutdown called but engine thread was already shutdown.");
		return;
	}
	logger->info("Shutting down engine.");
	bRunning = false;
	mainGameLoopThread.join();
}

Engine::~Engine()
{
	Shutdown();
}