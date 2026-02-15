#pragma once
#include <GL/glew.h>
#include <spdlog/spdlog.h>
#include <string>
#include <fstream>


namespace debugging
{
	inline bool checkOpenGLErrors()
	{
		GLenum err;
		while ((err = glGetError()) != GL_NO_ERROR)
		{
			spdlog::error("OpenGL errors!");
			spdlog::error("Error code: {}", err);
			return true;
		}
		return false;
	}
}
namespace helper
{
	inline bool loadFile(std::string fileName, std::string& outContent)
	{
		std::ifstream fileStream(fileName, std::ios::in);
		if (!fileStream.is_open())
		{
			spdlog::error("Failed to open file: {}", fileName);
			return false;
		}
		std::string line = "";
		outContent = "";
		while (std::getline(fileStream, line))
		{
			outContent += line + "\n";
		}
		fileStream.close();
		return true;
	}
}