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

namespace rendering
{
	struct TextureFlags
	{
		GLuint TextureWrapS = GL_REPEAT;
		GLuint TextureWrapT = GL_REPEAT;
		GLuint TextureMinFilter = GL_LINEAR_MIPMAP_LINEAR;
		GLuint TextureMagFilter = GL_LINEAR;
	};
}


class RenderEngine
{
	GLuint vert;
	GLuint frag;
	GLuint program;
	GLint uniformIndexMPV;
	GLint uniformIndexLightDir;
	GLint uniformIndexObjTransform;

	GLuint Deferred_vert_GeoPass;
	GLuint Deferred_frag_GeoPass;
	GLuint Deferred_vert_LightingPass;
	GLuint Deferred_frag_LightingPass;

	int FrameHistory;

	std::shared_ptr<Camera> RenderCamera;

	std::shared_ptr<spdlog::logger> logger;

	Shader ActiveShader;

	// later we will need a scene graph. That time is now :)
	std::vector<std::shared_ptr<GameObject>> RenderQueue;
	std::shared_ptr<SceneGraph> sceneGraph;

	// buffer that includes information about the screen
	// ideal: this is the same memory that opengl renders to for deferred rendering pipeline
	//torch::Tensor screen_buffer;

	// TODO: think about if this is where textures should live
	bool loadTexture(unsigned char* data, int width, int height, int channels, GLuint& texture, rendering::TextureFlags flags);
	bool loadTextureFromFile(std::string filePath, GLuint& texture, rendering::TextureFlags flags);


public:
	RenderEngine();
	~RenderEngine();
	virtual void initialize(std::string shaderPrefix, std::shared_ptr<SceneGraph> SceneGraph_ptr);
	virtual void RenderFrame(double Delta);
	inline void SetRenderCamera(std::shared_ptr<Camera> newCam) { RenderCamera = newCam; }
};

