#include "cpuBVH.h"
void cpuBVH::BuildBVH()
{
	logger->info("Building bvh for {} primitives", NumPrims); 
	auto& root = bvhNode[rootNodeIdx];
	// going to assume each Shape has a center precalculated
	root.left = root.right = -1;
	root.firstPrim = 0, root.primCount = NumPrims;
	UpdateNodeBounds(rootNodeIdx);
	Subdivide(rootNodeIdx);
	logger->info("BVH Nodes:");
	for (int i = 0; i < nodesUsed; i++)
	{
		auto node = bvhNode[i];
		logger->info("Node {}:\naabbMin: {}, {}, {}\naabbMax: {}, {}, {}\nleft: {} | right: {}\nfirstPrim: {} | primCount: {}",
			i,
			node.aabbMin.x, node.aabbMin.y, node.aabbMin.z, node.aabbMax.x, node.aabbMax.y, node.aabbMax.z,
			node.left, node.right,
			node.firstPrim, node.primCount);
	}
}
void cpuBVH::UpdateNodeBounds(unsigned int nodeIdx)
{
	auto& node = bvhNode[nodeIdx];
	node.aabbMin = glm::vec3(1e30f);
	node.aabbMax = glm::vec3(-1e30f);
	for (int first = node.firstPrim, i = 0; i < node.primCount; i++)
	{
		// get bounds of each primitive
		// the aabb has already been computed
		auto p = primitives[i + first];
		// need to find the max x, y, and z values independently to build the bounding box
		node.aabbMin = helper::minComponents(node.aabbMin, p.minCoords);
		node.aabbMax = helper::maxComponents(node.aabbMax, p.maxCoords);
	}
	logger->info("new aabb bounds (x axis): {} - {}", node.aabbMin.x, node.aabbMax.x);
	logger->info("new aabb bounds (y axis): {} - {}", node.aabbMin.y, node.aabbMax.y);
	logger->info("new aabb bounds (z axis): {} - {}", node.aabbMin.z, node.aabbMax.z);

}
void cpuBVH::Subdivide(unsigned int nodeIdx)
{
	auto& node = bvhNode[nodeIdx];
	if (node.primCount <= 2) 
	{
		logger->info("When subdividing, {} <= 2", node.primCount);
		return;
	}

	// get split axis and pos
	glm::vec3 extent = node.aabbMax - node.aabbMin;
	int axis = 0;
	if (extent.y > extent.x) axis = 1;
	if (extent.z > extent[axis]) axis = 2;
	float splitPos = node.aabbMin[axis] + extent[axis] * 0.5f;
	int i = node.firstPrim;
	int j = i + node.primCount - 1;
	// sort them based on the split
	logger->info("Split axis: {}", axis);
	while (i < j)
	{
		logger->info("Checking axis of prim {}\ncenter: {}\nsplit: {}", i, primitives[i].center[axis], splitPos);
		// this needs to be checking the centroid of the primitive
		if (primitives[i].center[axis] <= splitPos)
		{
			logger->info("Incrememting i");
			i++;
		}
		else
		{
			j--;

			logger->info("Swapping prims {}, {}\n{}'s center: {}\n{}'s center: {}", i, j,
				i, primitives[i].center[axis],
				j, primitives[j].center[axis]);			
			// swap
			auto t = primitives[i];
			primitives[i] = primitives[j];
			primitives[j] = t;
		}
	}
	int leftCount = i - node.firstPrim;
	if (leftCount == 0 || leftCount == node.primCount) return;
	// create child nodes
	nodesUsed += 1;
	int leftChildIdx = nodesUsed;
	nodesUsed += 1;
	int rightChildIdx = nodesUsed;
	bvhNode[leftChildIdx].firstPrim = node.firstPrim;
	bvhNode[leftChildIdx].primCount = leftCount;
	bvhNode[rightChildIdx].firstPrim = i;
	bvhNode[rightChildIdx].primCount = node.primCount - leftCount;
	if (leftChildIdx > MAX_PRIMS * 2 - 1 || rightChildIdx > MAX_PRIMS * 2 - 1)
	{
		logger->info("Ran out of nodes");
		return;
	}
	node.left = leftChildIdx;
	node.right = rightChildIdx;
	node.primCount = 0;
	logger->info("Adding child nodes to node: {}:\nleft: {} | right: {}", nodeIdx, leftChildIdx, rightChildIdx);
	UpdateNodeBounds(leftChildIdx);
	UpdateNodeBounds(rightChildIdx);
	Subdivide(leftChildIdx);
	Subdivide(rightChildIdx);
}

void cpuBVH::intersectBVH(Ray& ray, const unsigned int nodeIdx)
{
	BVHNode& node = bvhNode[nodeIdx];
	if (!intersectAABB(ray, node.aabbMin, node.aabbMax)) return;
	if (node.left == -1 && node.right == -1)
	{
		for (int i = 0; i < node.primCount; i++)
		{
			intersectPrim(ray, primitives[node.firstPrim + i]);
		}
	}
	else
	{
		if (node.left != -1 && node.left != nodeIdx)
		{
			intersectBVH(ray, node.left);
		}
		if (node.right != -1 && node.right != nodeIdx)
		{
			intersectBVH(ray, node.right);
		}
	}

}

bool cpuBVH::intersectAABB(const Ray& ray, const glm::vec3 bmin, const glm::vec3 bmax)
{
	float tx1 = (bmin.x - ray.origin.x) / ray.direction.x, 
		tx2 = (bmax.x - ray.origin.x) / ray.direction.x;
	float tmin = std::min(tx1, tx2), 
		tmax = std::max(tx1, tx2);
	float ty1 = (bmin.y - ray.origin.y) / ray.direction.y, 
		ty2 = (bmax.y - ray.origin.y) / ray.direction.y;
	tmin = std::max(tmin, std::min(ty1, ty2)), 
		tmax = std::min(tmax, std::max(ty1, ty2));
	float tz1 = (bmin.z - ray.origin.z) / ray.direction.z, 
		tz2 = (bmax.z - ray.origin.z) / ray.direction.z;

	tmin = std::max(tmin, std::min(tz1, tz2)), 
		tmax = std::min(tmax, std::max(tz1, tz2));

	if (!(tmax >= tmin && tmin < ray.t && tmax > 0))
	{
		logger->info("tmax: {} | tmin: {} | ray.t: {}\n{} | {} | {}", tmax, tmin, ray.t, tmax >= tmin, tmin <= ray.t, tmax > 0);
	}
	return tmax >= tmin && tmin < ray.t && tmax > 0;
}

void cpuBVH::intersectPrim(Ray& ray, const Primitive& primitive)
{
	// TODO: send the ray down the primitive's bvh
	// for now, we will just pretend like it hits if it hits the aabb
	if (intersectAABB(ray, primitive.minCoords, primitive.maxCoords))
	{
		// hit
		// TODO: this is not actually what we want to do, its just good enough for now
		auto dist = glm::distance(ray.origin, primitive.center);
		if (dist < ray.t)
		{
			ray.t = dist;
			ray.hitIdx = primitive.Idx;
		}

	}
}
