#pragma once
#include "RenderEngine.h"

class DeferredRenderEngine :
    public RenderEngine
{
	Shader GeoPassShader;
	Shader LightingPassShader;

public:
	/**
		magic variables :
		- prefix.geo.vert
		- prefix.geo.frag
		- prefix.light.vert
		- prefix.light.frag
	*/
	DeferredRenderEngine();
	void initialize(std::shared_ptr<SceneGraph> SceneGraph_ptr) override;
	void RenderFrame(double Delta) override;

};

