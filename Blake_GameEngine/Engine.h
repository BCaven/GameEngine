#pragma once
#include "Utility.h"
#include <thread>
#include "GameObject.h"
#include "RenderEngine.h"
#include "SceneGraph.h"

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
	std::vector <std::shared_ptr<GameObject>> TickableGameObjects;
	std::vector<std::shared_ptr<GameObject>> GameObjects;
	std::shared_ptr<SceneGraph> sceneGraph;

	std::thread mainGameLoopThread;

public:
	Engine();
	~Engine();

	void initialize();

	void MainGameLoop();

	void Shutdown();

	void RegisterGameObject(std::shared_ptr<GameObject> obj);

	template <typename T, typename... Args>
	void SpawnObject(Args... args)
	{
		// TODO: make sure we can only pass children of GameObject.
		auto newObj = std::make_shared<T>(args...);
		sceneGraph->AddObjectToSceneGraph(newObj);
		RegisterGameObject(newObj);
	}

	std::shared_ptr<SceneGraph> GetSceneGraph() { return sceneGraph; }
};

