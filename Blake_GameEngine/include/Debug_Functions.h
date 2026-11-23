#pragma once
#include <GL/glew.h>
#include <spdlog/spdlog.h>
#include <string>
#include <fstream>
#include <vector>
#include <functional>
#include <glm/glm.hpp>
#include "Utility.h"

#include <stb_image.h>

namespace debugging
{
	inline bool checkOpenGLErrors(const char* where = "", int line = 0)
	{
		bool any = false;
		for (GLenum err; (err = glGetError()) != GL_NO_ERROR; any = true)
			spdlog::error("GL error 0x{:x} at {}:{}", err, where, line);
		return any;
	}
	#define GL_ERRCHECK() checkOpenGLErrors(__FILE__, __LINE__)
	
	
	template <typename T, typename... Args>
	inline T timeIt(double& time, std::function<T(Args...)> func, Args... args)
	{
		double startTime = Utility::getTimeSeconds();
		T retVal = func(args...);
		double endTime = Utility::getTimeSeconds();
		time = endTime - startTime;
		return retVal;
	}
	

	inline static const char* dbgSource(GLenum s) {
		switch (s) {
		case GL_DEBUG_SOURCE_API:             return "API";
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   return "WindowSystem";
		case GL_DEBUG_SOURCE_SHADER_COMPILER: return "ShaderCompiler";
		case GL_DEBUG_SOURCE_THIRD_PARTY:     return "ThirdParty";
		case GL_DEBUG_SOURCE_APPLICATION:     return "Application";
		default:                              return "Other";
		}
	}
	inline static const char* dbgType(GLenum t) {
		switch (t) {
		case GL_DEBUG_TYPE_ERROR:               return "Error";
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "Deprecated";
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  return "UndefinedBehavior";
		case GL_DEBUG_TYPE_PORTABILITY:         return "Portability";
		case GL_DEBUG_TYPE_PERFORMANCE:         return "Performance";
		case GL_DEBUG_TYPE_MARKER:              return "Marker";
		default:                                return "Other";
		}
	}

	inline void GLAPIENTRY glDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
		GLsizei length, const GLchar* message, const void*)
	{
		switch (severity) {
		case GL_DEBUG_SEVERITY_HIGH:
			spdlog::error("[GL {}/{} #{}] {}", dbgSource(source), dbgType(type), id,
				std::string_view(message, length));
			break;
		case GL_DEBUG_SEVERITY_MEDIUM:
			spdlog::warn("[GL {}/{} #{}] {}", dbgSource(source), dbgType(type), id,
				std::string_view(message, length));
			break;
		default:
			spdlog::info("[GL {}/{} #{}] {}", dbgSource(source), dbgType(type), id,
				std::string_view(message, length));
		}

#ifndef NDEBUG
		if (type == GL_DEBUG_TYPE_ERROR) {
#ifdef _MSC_VER
			__debugbreak();            // synchronous output means the stack points at the bad call
#else
			std::raise(SIGTRAP);       // needs <csignal>
#endif
		}
#endif
	}

	inline void initGLDebug()
	{
		GLint flags = 0;
		glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
		if (!(flags & GL_CONTEXT_FLAG_DEBUG_BIT))
			spdlog::warn("Not a debug context; driver may emit few or no debug messages");

		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);   // callback fires inside the offending call
		glDebugMessageCallback(glDebugCallback, nullptr);

		// Drop the noisy "buffer X will use VIDEO memory" style notifications
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION,
			0, nullptr, GL_FALSE);
	}

}
namespace helper
{
	inline bool loadImageFile(std::string fileName, int& width, int& height, int& channels, unsigned char*& data)
	{
		data = stbi_load(fileName.c_str(), &width, &height, &channels, 0);
		if (!data)
		{
			return false;
		}
		return true;
	}
	inline bool freeImage(unsigned char*& data)
	{
		stbi_image_free(data);
		return true;
	}

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

	template<typename T>
	inline bool approximately(T inputVal, T testVal)
	{
		T approx = 0.0001;
		if (inputVal - approx < testVal)
		{
			if (inputVal + approx > testVal)
			{
				return true;
			}
		}
		return false;
	}

	inline static glm::vec3 maxComponents(glm::vec3 first, glm::vec3 second)
	{
		glm::vec3 r(0);
		r.x = fmaxf(first.x, second.x);
		r.y = fmaxf(first.y, second.y);
		r.z = fmaxf(first.z, second.z);
		return r;
	}
	inline static glm::vec3 minComponents(glm::vec3 first, glm::vec3 second)
	{
		glm::vec3 r(0);
		r.x = fminf(first.x, second.x);
		r.y = fminf(first.y, second.y);
		r.z = fminf(first.z, second.z);
		return r;
	}
}