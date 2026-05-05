#pragma once
#include <memory>
#include <algorithm>
#include "Shape.h"
#include <glm/vec3.hpp>

#define MAX_PRIMS 100

struct BVHNode
{
	// TODO: gpu version of this will need to be a multiple of 32 bytes for memory performance
	glm::vec3 aabbMin, aabbMax = glm::vec3(-1);
	int left, right = -1;
	int firstPrim, primCount = -1;
};
struct Primitive
{
	glm::vec3 center = glm::vec3(-1);
	glm::vec3 maxCoords = glm::vec3(-1);
	glm::vec3 minCoords = glm::vec3(-1);
	// should also have a pointer to the
	// shape/thing that this is
	int Idx = -1;

	Primitive(int idx, 
		glm::vec3 Center, 
		glm::vec3 AABBMax, 
		glm::vec3 AABBMin) :
		Idx(idx), center(Center), maxCoords(AABBMax), minCoords(AABBMin)
	{}
	Primitive() {}
};
struct Ray
{
	glm::vec3 origin = glm::vec3(-1), direction = glm::vec3(-1);
	float t = 1e30f;
	int hitIdx = -1;
};

class cpuBVH
{
	std::shared_ptr<spdlog::logger> logger;

	// TODO: a better, more-sane way of dealing with this, 
	// maybe dont have the bvh own the prim list
public:
	int NumPrims = 0;
	BVHNode bvhNode[MAX_PRIMS * 2 - 1];
	Primitive primitives[MAX_PRIMS];
	unsigned int rootNodeIdx = 0 ;
	unsigned int nodesUsed = 1;

	cpuBVH() : bvhNode{}, primitives{}
	{
		logger = spdlog::get("cpu_bvh");
		if (!logger)
		{
			logger = spdlog::stdout_color_mt("cpu_bvh");
		}
	}
	~cpuBVH()
	{

	}

	void BuildBVH();

	void UpdateNodeBounds(unsigned int nodeIdx);

	void Subdivide(unsigned int nodeIdx);

	void intersectBVH(Ray& ray, const unsigned int nodeIdx);

	bool intersectAABB(const Ray& ray, const glm::vec3 bmin, const glm::vec3 bmax);

	void intersectPrim(Ray& ray, const Primitive& primitive);

};

