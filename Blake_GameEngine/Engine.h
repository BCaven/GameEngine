#pragma once
#include "Utility.h"
#include <thread>
#include "GameObject.h"

class Engine
{
	bool bRunning = false;
	int PrevFrameTimeMillis = 0;
	std::vector<GameObject*> TickableGameObjects;
	std::vector<GameObject*> GameObjects;


	std::thread mainGameLoopThread;

public:
	void initialize();

	void MainGameLoop();

	void Shutdown();
};

