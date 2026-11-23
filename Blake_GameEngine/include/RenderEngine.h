#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

//#include <torch/torch.h>

#include "Debug_Functions.h"
#include "GameObject.h"
#include "Utility.h"
#include "SceneGraph.h"
#include "Camera.h"
#include "Shader.h"

class RenderEngine
{
protected:
	int FrameHistory;

	std::shared_ptr<Camera> RenderCamera;

	std::shared_ptr<spdlog::logger> logger;

	// later we will need a scene graph. That time is now :)
	std::vector<std::shared_ptr<GameObject>> RenderQueue;
	std::shared_ptr<SceneGraph> sceneGraph;

	// buffer that includes information about the screen
	// ideal: this is the same memory that opengl renders to for deferred rendering pipeline
	//torch::Tensor screen_buffer;
	// this might be better to have as a child-class-only member

	

public:
	virtual ~RenderEngine() = default;
	virtual void initialize(std::shared_ptr<SceneGraph> SceneGraph_ptr) = 0;
	virtual void RenderFrame(double Delta) = 0;
	void SetRenderCamera(std::shared_ptr<Camera> newCam) { RenderCamera = newCam; }
	virtual void bindTex(std::shared_ptr<Texture> tex) = 0;
};

