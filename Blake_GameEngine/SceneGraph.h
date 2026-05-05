#pragma once
#include "GameObject.h"
#include "Camera.h"
#include <vector>

/*
Bounding volume hierarchy



*/

class SceneGraph
{
protected:
	std::shared_ptr<Camera> activeCamera;
	std::vector<std::shared_ptr<GameObject>> GameObjects;
	std::shared_ptr<spdlog::logger> logger;


public:

	SceneGraph();
	~SceneGraph();

	virtual inline void AddObjectToSceneGraph(std::shared_ptr<GameObject> newObj) { GameObjects.push_back(newObj); }
	virtual std::vector<std::shared_ptr<GameObject>> GetVisibleObjects();
	virtual std::vector<std::shared_ptr<GameObject>> GetAllObjects();
};

