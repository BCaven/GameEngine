#include "SceneGraphBVH_cpu.h"

void SceneGraphBVH_cpu::AddObjectToSceneGraph(std::shared_ptr<GameObject> newObj)
{
	// going to have to compute new primitive then rebuild the bvh
	auto shape = newObj->getShape();
	glm::vec3 min, max;
	shape->getAABB(min, max);

	// translate to world coordinates
	glm::vec4 min4(min, 1.0f), max4(max, 1.0f);
	auto model = newObj->getModel();
	glm::vec3 worldMin(min4 * model);
	glm::vec3 worldMax(max4 * model);
	// calc center (just doing the bvh center)
	glm::vec3 center = worldMin + ((worldMax - worldMin) / 2.0f);
	bvh.primitives[bvh.NumPrims].maxCoords = worldMax;
	bvh.primitives[bvh.NumPrims].minCoords = worldMin;
	bvh.primitives[bvh.NumPrims].center = center;

	GameObjects.push_back(newObj);
	bvh.primitives[bvh.NumPrims].Idx = GameObjects.size() - 1;
	//logger->info("center of primitive {}", center);
	bvh.NumPrims += 1;

	// rebuild bvh
	bvh.BuildBVH();
}

#define X_WIDTH 30.0f
#define Y_WIDTH 1.0f
#define FRUST_WIDTH 1.0f
#define FRUST_HEIGHT 1.0f
std::vector<std::shared_ptr<GameObject>> SceneGraphBVH_cpu::GetVisibleObjects()
{
	// shoot rays out of active camera
	// this should be one per pixel but aint no way we are
	// doing that single threaded on the cpu
	Ray ray;
	auto camOrigin = activeCamera->GetPosition();
	auto camForward = activeCamera->GetForward();
	auto camUp = activeCamera->GetUp() * FRUST_HEIGHT;
	auto camRight = glm::cross(camForward, camUp) * FRUST_WIDTH;
	// need to get top left, top right and bottom left points
	glm::vec3 origin = glm::normalize(camForward + camUp + camRight);
	glm::vec3 max_x = glm::normalize(camForward + camUp - camRight);
	glm::vec3 max_y = glm::normalize(camForward - camUp + camRight);
	std::vector<std::shared_ptr<GameObject>> VisibleObjects;
	// shoot rays out 
	for (int x = 0; x < X_WIDTH; x++) for (int y = 0; y < Y_WIDTH; y++)
	{
		// calc the ray for the root node
		// pixel pos in 3D space:
		// origin = (0,0) screen space translated to world space
		// max_x = (X_WIDTH, 0) screen space translated to world space
		// max_y = (0, Y_WIDTH) screen space translated to world space
		// origin + (max_x - origin) * (x / X_WIDTH) + (max_y - origin) * (y / Y_WIDTH);
		auto rayDir = origin + (max_x - origin) *
			(x / X_WIDTH) + (max_y - origin) * (y / Y_WIDTH);
		ray.direction = rayDir;
		ray.hitIdx = -1;
		ray.t = 1000;
		bvh.intersectBVH(ray, 0);
		if (ray.hitIdx != -1 && ray.hitIdx < GameObjects.size())
		{
			//logger->info("Hit!");
			VisibleObjects.push_back(GameObjects[ray.hitIdx]);
		}
	}
	return VisibleObjects;
}

