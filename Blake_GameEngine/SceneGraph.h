#pragma once
#include "GameObject.h"
#include <vector>

/*
Bounding volume hierarchy



*/

class SceneGraph
{
	std::vector<std::shared_ptr<GameObject>> GameObjects;

public:

	SceneGraph();
	~SceneGraph();

	inline void AddObjectToSceneGraph(std::shared_ptr<GameObject> newObj) { GameObjects.push_back(newObj); }
	std::vector<std::shared_ptr<GameObject>> GetVisibleObjects();
	std::vector<std::shared_ptr<GameObject>> GetAllObjects();
};

