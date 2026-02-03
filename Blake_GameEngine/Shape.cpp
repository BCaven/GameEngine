#include "Shape.h"

Shape::Shape(const size_t triangleCount, const std::vector<float>& data)
{
	logger = spdlog::get("shape");
	if (!logger)
	{
		logger = spdlog::stdout_color_mt("shape");
	}

	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * data.size(), &data[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	size_t offset = triangleCount * 3 * ( 3 * sizeof(float)); // Offset: number of vertices = num triangles * 3 * 3
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*) offset);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	if (debugging::checkOpenGLErrors())
	{
		logger->warn("OpenGL errors when creating the shape!");
	}

	// push the data into the pos and norm vectors (pos first, then normals)
	for (int i = 0; i < triangleCount * 3; i+=1)
	{
		int current = i * 3;
		float x = data[current];
		float y = data[current + 1];
		float z = data[current + 2];
		pos.push_back(glm::vec3(x, y, z));
	}
	for (size_t i = 0; i < triangleCount * 3; i += 1)
	{
		// offset: three points per triangle, three floats per point = 9 * num triangles
		size_t current = (i * 3) + (triangleCount * 9);
		float x = data[current];
		float y = data[current + 1];
		float z = data[current + 2];
		norm.push_back(glm::vec3(x, y, z));
	}
}
Shape::~Shape()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	vao = 0;
	vbo = 0;
	if (debugging::checkOpenGLErrors())
	{
		logger->warn("OpenGL errors when deleting shape");
	}
}

std::shared_ptr<Shape> Shape::fromFile(std::string fileName)
{
	std::fstream input(fileName);
	std::vector<float> floatData;
	if (input.is_open())
	{
		// file is organized: triangle count, triangles (x, y, z), normals (x, y, z)

		std::string line;
		// get the triangle count
		std::getline(input, line, ' ');
		int triCount = std::stoi(line);
		while (std::getline(input, line, ' '))
		{
			float x = std::stof(line);
			floatData.push_back(x);
		}

		std::shared_ptr<Shape> constructedShape(new Shape(triCount, floatData));
		return constructedShape;
	}
	else
	{
		auto logger = spdlog::get("shape");
		if (!logger)
		{
			logger = spdlog::stdout_color_mt("shape");
		}
		logger->error("Failed to open file: {}", fileName);
	}
	return nullptr;
}