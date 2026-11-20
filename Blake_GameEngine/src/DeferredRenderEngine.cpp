#include "DeferredRenderEngine.h"

DeferredRenderEngine::DeferredRenderEngine(std::string shaderPrefix) : GeoPassShader(shaderPrefix + ".geo"), LightingPassShader(shaderPrefix + ".light")
{
	

}

void DeferredRenderEngine::initialize(std::shared_ptr<SceneGraph> SceneGraph_ptr)
{
	// TODO: inputs for geo and light shaders

}

void DeferredRenderEngine::RenderFrame(double Delta)
{
	// geometry pass first

	// then lighting pass
}