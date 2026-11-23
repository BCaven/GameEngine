#pragma once
#include "RenderEngine.h"

class DeferredRenderEngine : public RenderEngine
{
	Shader GeoPassShader;
	Shader LightingPassShader;

	rendering::gBuffer gbuffer;

	GLuint lightingOut = 0;
	GLuint lightingFbo = 0;

public:
	/**
		magic variables :
		- prefix.geo.vert
		- prefix.geo.frag
		- prefix.light.vert
		- prefix.light.frag
	*/
	DeferredRenderEngine() : DeferredRenderEngine("shaders/deferred") {};
	DeferredRenderEngine(std::string shaderPrefix);
	void initialize(std::shared_ptr<SceneGraph> SceneGraph_ptr) override;
	void RenderFrame(double Delta) override;

	/**
	For deferred I do not think the lighting pass will see anything that is not from the gBuffer

	TODO: this should either be a better wrapper or be removed - it is confusing
	*/
	inline void bindTex(std::shared_ptr<Texture> tex) override
	{
		auto& ActiveShader = GeoPassShader; //geo ? GeoPassShader : LightingPassShader;
		if (tex)
		{
			ActiveShader.SetInput("bTex", true);
			glBindTexture(GL_TEXTURE_2D, tex->get());
		}
		else
		{
			ActiveShader.SetInput("bTex", false);
		}
	}
};

