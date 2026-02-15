#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include "Debug_Functions.h"
#include "Shape.h"
#include "Utility.h"


class RenderEngine
{
	GLuint vert;
	GLuint frag;
	GLuint program;
	GLint uniformIndex;

	std::shared_ptr<spdlog::logger> logger;

	// later we will need a scene graph, but for now going to pretend liek there is an array of
	// shapes that need to be rendered.
	std::vector<std::shared_ptr<Shape>> RenderQueue;

	void loadShaders(std::string shaderName);

	public:
	RenderEngine();
	~RenderEngine();
	void initialize();
	void RenderFrame(double Delta);
	void addToRenderQueue(std::shared_ptr<Shape> shape) { RenderQueue.push_back(shape); }
};

