#include "SceneGraph.h"

SceneGraph::SceneGraph()
{
	logger = spdlog::get("SceneGraph");
	if (!logger)
	{
		logger = spdlog::stdout_color_mt("SceneGraph");
	}
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
