#include "Engine.h"


void Engine::initialize()
{
	// Initialization code here
	bRunning = true;

	// construct our scene graph + register game objects that tick
	// for now, just going to put them in an array
	for (GameObject* obj : GameObjects)
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
	const float MillisToSeconds = 1 / 1000.0f;
	while (bRunning)
	{
		int deltaTimeMillis = Utility::getDeltaTimeMillis();
		float deltaMillis = deltaTimeMillis - PrevFrameTimeMillis;
		PrevFrameTimeMillis = deltaTimeMillis;

		for (auto obj : TickableGameObjects)
		{
			obj->Tick(deltaMillis * MillisToSeconds); // Convert to seconds
		}

		// TODO: whatever else needs to happen each frame

		// TBD: Might be able to be removed later
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
	
}

void Engine::Shutdown()
{
	bRunning = false;
	mainGameLoopThread.join();
}