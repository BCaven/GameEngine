#pragma once
#include <GL/glew.h>
#include <spdlog/spdlog.h>

namespace debugging
{
	inline bool checkOpenGLErrors()
	{
		GLenum err;
		while ((err = glGetError()) != GL_NO_ERROR)
		{
			spdlog::error("OpenGL errors!");
			return true;
		}
		return false;
	}
}