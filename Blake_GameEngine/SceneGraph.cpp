#include "SceneGraph.h"

SceneGraph::SceneGraph()
{

}
SceneGraph::~SceneGraph()
{

}

std::vector<std::shared_ptr<GameObject>> SceneGraph::GetVisibleObjects()
{
	std::vector<std::shared_ptr<GameObject>> visibleObjects;
	for (auto obj : GameObjects)
	{
		if (obj->IsVisible())
		{
			visibleObjects.push_back(obj);
		}
	}
	return visibleObjects;
}

std::vector<std::shared_ptr<GameObject>> SceneGraph::GetAllObjects()
{
	return GameObjects;
}
