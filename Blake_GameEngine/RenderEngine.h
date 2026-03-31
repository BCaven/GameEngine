#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include <torch/torch.h>

#include "Debug_Functions.h"
#include "GameObject.h"
#include "Utility.h"
#include "SceneGraph.h"
#include "Camera.h"


class RenderEngine
{
	GLuint vert;
	GLuint frag;
	GLuint program;
	GLint uniformIndexMPV;
	GLint uniformIndexLightDir;
	GLint uniformIndexObjTransform;

	int FrameHistory;

	std::shared_ptr<Camera> RenderCamera;

	std::shared_ptr<spdlog::logger> logger;

	// later we will need a scene graph. That time is now :)
	std::vector<std::shared_ptr<GameObject>> RenderQueue;
	std::shared_ptr<SceneGraph> sceneGraph;

	// buffer that includes information about the screen
	// ideal: this is the same memory that opengl renders to for deferred rendering pipeline
	torch::Tensor screen_buffer;

	void loadShaders(std::string shaderName);
	bool loadShader(std::string shaderFile, GLuint shader);

public:
	RenderEngine();
	~RenderEngine();
	void initialize(std::string shaderName, std::shared_ptr<SceneGraph> SceneGraph_ptr);
	void RenderFrame(double Delta);
	inline void SetRenderCamera(std::shared_ptr<Camera> newCam) { RenderCamera = newCam; }
};

