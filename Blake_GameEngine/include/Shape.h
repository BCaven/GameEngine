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
	GLuint tex;
	std::shared_ptr<spdlog::logger> logger;

	std::shared_ptr<Shader> ActiveShader;

	bool loadTextureFromFile(std::string filePath, GLuint& texture, rendering::TextureFlags flags);
	bool loadTexture(unsigned char* data, int width, int height, int channels, GLuint& texture, rendering::TextureFlags flags);

	Shape(const size_t vertCount, const std::vector<float>& data);

public:
	// need to make this idiot proof
	template <typename T>
	static std::shared_ptr<Shape> fromFile(std::string fileName, T shader);

	//Shape(const size_t vertCount, const std::vector<float>& data) : Shape(vertCount, data, "simple") {};
	Shape(const size_t vertCount, const std::vector<float>& data, std::shared_ptr<Shader> ShaderPtr) : Shape(vertCount, data) 
	{
		ActiveShader = ShaderPtr;
	};
	Shape(const size_t vertCount, const std::vector<float>& data, std::string ShaderPrefix) : Shape(vertCount, data)
	{
		ActiveShader = std::make_shared<Shader>(ShaderPrefix);
	};

	~Shape();
	inline GLuint getVAO() 
	{
		return vao;
	};
	inline size_t getVertexCount()
	{
		return pos.size();
	}

	inline void bindTex()
	{
		if (tex != -1)
		{
			ActiveShader->SetInput("bTex", true);
			glBindTexture(GL_TEXTURE_2D, tex);
			glBindVertexArray(vao);
		}
	}

	void getAABB(glm::vec3& min, glm::vec3& max);
};

