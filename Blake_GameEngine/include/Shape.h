#pragma once
#include <GL/glew.h>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <vector>
#include <memory>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <string>
#include <fstream>

#include "Debug_Functions.h"
#include "Utility.h"
#include "Shader.h"
#include "Texture.h"

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
	std::shared_ptr<Texture> texture = nullptr;
	std::shared_ptr<spdlog::logger> logger;

	// TODO later: support different shaders on each shape in the geo pass
	// this might be an absolute nightmare and not worth it :)
	//std::shared_ptr<Shader> ActiveShader;


public:
	// need to make this idiot proof
	template <typename T>
	static std::shared_ptr<Shape> fromFile(std::string fileName, T shader);
	static std::shared_ptr<Shape> fromFile(std::string fileName);

	Shape(const size_t vertCount, const std::vector<float>& data);

	/*
	//Shape(const size_t vertCount, const std::vector<float>& data) : Shape(vertCount, data, "simple") {};
	Shape(const size_t vertCount, const std::vector<float>& data, std::shared_ptr<Shader> ShaderPtr) : Shape(vertCount, data)
	{
		ActiveShader = ShaderPtr;
	};
	Shape(const size_t vertCount, const std::vector<float>& data, std::string ShaderPrefix) : Shape(vertCount, data)
	{
		ActiveShader = std::make_shared<Shader>(ShaderPrefix);
	};
	//*/

	~Shape();
	inline GLuint getVAO() 
	{
		return vao;
	};
	inline size_t getVertexCount()
	{
		return pos.size();
	}

	inline void setTexture(std::shared_ptr<Texture> tex) { texture = tex; }

	inline std::shared_ptr<Texture> getTexture()
	{
		return texture;
	}

	void getAABB(glm::vec3& min, glm::vec3& max);
};

