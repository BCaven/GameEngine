#pragma once
#include <GL/glew.h>
#include <spdlog/spdlog.h>
#include <string>
#include <fstream>
#include <vector>


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
	/*
	template <typename Callable, typename... Args, typename T>
	inline T timeIt(double& time, Callable func, Args... args)
	{
		double startTime = Utility::getTimeSeconds();
		T retVal = func(args...);
		double endTime = Utility::getTimeSeconds();
		time = endTime - startTime;
		return retVal;
	}
	*/
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
	
	inline std::vector<std::string> split(const std::string& str, char delimiter) {
		std::vector<std::string> tokens(0);
		std::stringstream ss(str);
		std::string token;
		while (std::getline(ss, token, delimiter)) {
			tokens.push_back(token);
		}
		return tokens;
	}
}