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
	tex = -1;
	logger = spdlog::get("shape");
	if (!logger)
	{
		logger = spdlog::stdout_color_mt("shape");
	}
	if (data.size() != vertCount * 8)
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
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	size_t offset = vertCount * ( 3 * sizeof(float)); // Offset: number of floats = num vertices * 3
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*) offset);
	offset = vertCount * (3 * 2 * sizeof(float)); // Offset: number of floats = num vertices * 3 * 2
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*) offset);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	auto TexFlags = rendering::TextureFlags();
	loadTextureFromFile("default_texture.jpg", tex, TexFlags);

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

template <typename T>
std::shared_ptr<Shape> Shape::fromFile(std::string fileName, T shader)
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
			floatData.reserve(vertCount * 8); // grab room for UVs too even if we do not write them
		}
		else
		{
			floatData.push_back(std::stof(line));
		}
	}
	
	if (floatData.size() == vertCount * 2 * 3)
	{
		// fill the rest with dummy UV coordinates (0, 0), (0, 1), (1, 0) over and over
		if (vertCount % 3 != 0)
		{
			logger->warn("Our vertex count is not a multiple of 3 which suggests we do not have triangles");
		}
		// filling in three verticies at a time
		for (int i = 0; i < vertCount; i += 3)
		{
			floatData.insert(floatData.end(), { 0, 0, 0, 1, 1, 0 });
		}
	}

	if (floatData.size() != vertCount * 2 * 3 && floatData.size() != vertCount * 8)
	{
		logger->error("Failed to parse {}, expected {} or {} floats but got {}", fileName, vertCount * 2 * 3, vertCount * 8, floatData.size());
		return nullptr;
	}

	logger->info("Finished reading file, time: {}", Utility::getTimeSeconds() - startTime);
	startTime = Utility::getTimeSeconds();
	std::shared_ptr<Shape> constructedShape = std::make_shared<Shape>(vertCount, floatData, shader);
	logger->info("Finished constructing shape, time: {}", Utility::getTimeSeconds() - startTime);
	return constructedShape;
}
template std::shared_ptr<Shape> Shape::fromFile<std::string>(std::string fileName, std::string shader);
template std::shared_ptr<Shape> Shape::fromFile<const char*>(std::string fileName, const char* shader);
template std::shared_ptr<Shape> Shape::fromFile<std::shared_ptr<Shader>>(std::string fileName, std::shared_ptr<Shader> shader);



bool Shape::loadTextureFromFile(std::string filePath, GLuint& texture, rendering::TextureFlags flags)
{
	int width, height, channels;
	unsigned char* data = nullptr;
	if (!helper::loadImageFile(filePath, width, height, channels, data))
	{
		return false;
	}
	bool r = loadTexture(data, width, height, channels, texture, flags);
	helper::freeImage(data);
	return r;
}

bool Shape::loadTexture(unsigned char* data, int width, int height, int channels, GLuint& texture, rendering::TextureFlags flags)
{
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	// parse flags
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, flags.TextureWrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, flags.TextureWrapT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, flags.TextureMinFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, flags.TextureMagFilter);

	if (!data)
	{
		logger->warn("Failed to load data when trying to load texture!");
		return false;
	}
	// TODO: support for non-RGB textures
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	return true;
}
