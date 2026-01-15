#pragma once
#include <GL/glew.h>
#include <glm/vec3.hpp>
#include <vector>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <string>
#include <fstream>

class Shape
{
private:
	std::vector<glm::vec3> pos;
	std::vector<glm::vec3> norm;
	GLuint vao;
	GLuint vbo;
	std::shared_ptr<spdlog::logger> logger;

public:
	static std::shared_ptr<Shape> fromFile(std::string fileName);

	Shape(const size_t triangleCount, const std::vector<float>& data);
	~Shape();
	GLuint getVAO();
	GLsizei getVertexCount();
};

