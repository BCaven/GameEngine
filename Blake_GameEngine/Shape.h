#pragma once
#include <GL/glew.h>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <vector>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <string>
#include <fstream>

#include "Debug_Functions.h"

/*
TODO: make shapes instance-able

even though these are technically an abstraction on top of the mesh data stored on the gpu


*/

class Shape
{
private:
	std::vector<glm::vec3> pos;
	std::vector<glm::vec3> norm;
	std::vector<glm::vec2> uv;
	GLuint vao;
	GLuint vbo;
	std::shared_ptr<spdlog::logger> logger;

public:
	static std::shared_ptr<Shape> fromFile(std::string fileName);

	Shape(const size_t vertCount, const std::vector<float>& data);
	Shape(const size_t vertCount, const std::vector<float>& data, const std::vector<float>& uvCoords);

	~Shape();
	inline GLuint getVAO() 
	{
		return vao;
	};
	inline size_t getVertexCount()
	{
		return pos.size();
	}
};

