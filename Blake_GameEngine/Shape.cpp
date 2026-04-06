#include "Shape.h"
#include "Utility.h"
#include "Debug_Functions.h"


/*
TODO: 
-texture coordinates
-shape-specific shaders?
-at the very least, the shape needs to specify what texture it uses
-and this needs to be stored in the file

*/

Shape::Shape(const size_t vertCount, const std::vector<float>& data)
{
	vao = -1;
	vbo = -1;
	logger = spdlog::get("shape");
	if (!logger)
	{
		logger = spdlog::stdout_color_mt("shape");
	}
	if (data.size() != vertCount * 3 * 2)
	{
		logger->error("Data size does not match expected size for given vertex count!");
		return;
	}
	logger->info("Building shape with {} verts in {} floats", vertCount, data.size());
	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * data.size(), &data[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	size_t offset = vertCount * ( 3 * sizeof(float)); // Offset: number of floats = num vertices * 3
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*) offset);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	if (debugging::checkOpenGLErrors())
	{
		logger->warn("OpenGL errors when creating the shape!");
	}
	logger->info("Creating shape with {} vertices",	vertCount);
	// push the data into the pos and norm vectors (pos first, then normals)
	for (int i = 0; i < vertCount; i+=1)
	{
		int current = i * 3;
		float x = data[current];
		float y = data[current + 1];
		float z = data[current + 2];
		pos.push_back(glm::vec3(x, y, z));
	}
	for (size_t i = 0; i < vertCount; i += 1)
	{
		// offset: three floats per point = 3 * num num verts
		size_t current = (i * 3) + (vertCount * 3);
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
	auto logger = spdlog::get("shape-loader");
	if (!logger)
	{
		logger = spdlog::stdout_color_mt("shape-loader");
	}
	// TODO: convert it to binary
	logger->info("Loading shape from file: {}", fileName);
	auto startTime = Utility::getTimeSeconds();
	logger->info("Start time: {}", startTime);
	std::string rawFile;
	if (!helper::loadFile(fileName, rawFile))
	{
		logger->error("Failed to open {}", fileName);
		return nullptr;
	}
	int vertCount = -1;
	std::vector<float> floatData;

	auto splitLines = helper::split(rawFile, ' ');

	for (auto line : splitLines)
	{
		if (vertCount == -1)
		{
			vertCount = std::stoi(line);
			floatData.reserve(vertCount * 2 * 3);
		}
		else
		{
			floatData.push_back(std::stof(line));
		}
	}
	if (floatData.size() != vertCount * 2 * 3)
	{
		logger->error("Failed to parse {}, expected {} floats but got {}", fileName, vertCount * 2 * 3, floatData.size());
		return nullptr;
	}
	logger->info("Finished reading file, time: {}", Utility::getTimeSeconds() - startTime);
	startTime = Utility::getTimeSeconds();
	std::shared_ptr<Shape> constructedShape(new Shape(vertCount, floatData));
	logger->info("Finished constructing shape, time: {}", Utility::getTimeSeconds() - startTime);
	return constructedShape;
}