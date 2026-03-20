#include "SceneGraph.h"

SceneGraph::SceneGraph()
{

}
SceneGraph::~SceneGraph()
{

}

std::vector<std::shared_ptr<GameObject>> SceneGraph::GetVisibleObjects()
{
	return GameObjects;
}

std::vector<std::shared_ptr<GameObject>> SceneGraph::GetAllObjects()
{
	return GameObjects;
}
