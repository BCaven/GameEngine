#pragma once
#include "Utility.h"
#include <thread>
#include "GameObject.h"
#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec3.hpp>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

class Engine
{
	std::shared_ptr<spdlog::logger> logger;

	bool bRunning = false;
	Uint64 PrevFrameTimeMillis = 0;
	std::vector<GameObject*> TickableGameObjects;
	std::vector<GameObject*> GameObjects;


	std::thread mainGameLoopThread;

public:
	Engine();
	~Engine();

	void initialize();

	void MainGameLoop();

	void Shutdown();

	void RegisterGameObject(GameObject* obj);
};

