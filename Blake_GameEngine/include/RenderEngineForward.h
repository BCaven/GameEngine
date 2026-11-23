#pragma once
#include "RenderEngine.h"

class ForwardRenderEngine : public RenderEngine
{
	Shader ActiveShader;

	// buffer that includes information about the screen
	// ideal: this is the same memory that opengl renders to for deferred rendering pipeline
	//torch::Tensor screen_buffer;

	

public:
	ForwardRenderEngine() : ActiveShader("shaders/simple") 
	{
		logger = spdlog::get("forward_render_engine");
		if (!logger)
		{
			logger = spdlog::stdout_color_mt("forward_render_engine");
		}
		RenderCamera = nullptr;
	};
	~ForwardRenderEngine() {}
	void initialize(std::shared_ptr<SceneGraph> SceneGraph_ptr) override;
	void RenderFrame(double Delta) override;
	void bindTex(std::shared_ptr<Texture> tex) override;
};

