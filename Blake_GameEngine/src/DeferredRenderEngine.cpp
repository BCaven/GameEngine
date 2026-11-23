#include "DeferredRenderEngine.h"

DeferredRenderEngine::DeferredRenderEngine(std::string shaderPrefix) : 
	GeoPassShader(shaderPrefix + ".geo"), 
	LightingPassShader(shaderPrefix + ".light", COMPUTE)
{
	logger = spdlog::get("deferred_render_engine");
	if (!logger)
	{
		logger = spdlog::stdout_color_mt("deferred_render_engine");
	}
	lightingFbo = 0;
	lightingOut = 0;
}

void DeferredRenderEngine::initialize(std::shared_ptr<SceneGraph> SceneGraph_ptr)
{
	debugging::checkOpenGLErrors("initialize: entry", __LINE__);   // anything before this isn't your code

	// TODO: inputs for geo and light shaders
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	GeoPassShader.AddInput("mpv");
	GeoPassShader.AddInput("objTransform");

	// Lighting pass is going to be raymarched? or maybe raytraced?
	// going to do radiance cascades + surfels I think?
	sceneGraph = SceneGraph_ptr;

	// TODO: remember to delete and resize on window change
	GLint dims[4];
	glGetIntegerv(GL_VIEWPORT, dims);
	spdlog::info("viewport: {} {} {} {}", dims[0], dims[1], dims[2], dims[3]);

	gbuffer.Create(dims[2], dims[3]);
	debugging::checkOpenGLErrors("after gbuffer.Create", __LINE__);

	glGenFramebuffers(1, &lightingFbo);
	glBindFramebuffer(GL_FRAMEBUFFER, lightingFbo);
	rendering::makeTex2D(gbuffer.width, gbuffer.height, lightingOut, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, GL_COLOR_ATTACHMENT0);
	debugging::checkOpenGLErrors("after lightingFbo", __LINE__);
	spdlog::info("lightingFbo status: 0x{:x}", glCheckFramebufferStatus(GL_FRAMEBUFFER));
}

void DeferredRenderEngine::RenderFrame(double Delta)
{
	debugging::checkOpenGLErrors("start of RenderFrame", __LINE__);
	RenderQueue = sceneGraph->GetAllObjects();
	if (!RenderCamera)
	{
		logger->error("No camera selected!");
		return;
	}
	glm::mat4 view_projection = RenderCamera->BuildCameraMatrix();

	// geometry pass first
	glBindFramebuffer(GL_FRAMEBUFFER, gbuffer.fbo);
	glViewport(0, 0, gbuffer.width, gbuffer.height);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glClearColor(0.f, 0.f, 0.f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	GeoPassShader.Use();
	GeoPassShader.SetInput("mpv", view_projection);

	for (const auto& obj : RenderQueue)
	{
		if (!obj)
		{
			logger->error("Bad pointer to game object!");
			continue;
		}
		auto shape = obj->getShape();
		bindTex(shape->getTexture());

		glBindVertexArray(shape->getVAO());
		// bind texture if we have it

		glm::mat4 model_matrix = obj->getModel();
		GeoPassShader.SetInput("objTransform", model_matrix);

		glDrawArrays(GL_TRIANGLES, 0, shape->getVertexCount());
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	debugging::checkOpenGLErrors("After geometry pass", 79);
	// then lighting pass
	// bind gbuffer
	LightingPassShader.Use();
	LightingPassShader.SetInput("invViewProj", glm::inverse(view_projection));

	// gBuffer
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gbuffer.albedo);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gbuffer.normal);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, gbuffer.depth);
	LightingPassShader.SetInput("gAlbedo", 0);
	LightingPassShader.SetInput("gNormal", 1);
	LightingPassShader.SetInput("gDepth", 2);
	debugging::checkOpenGLErrors("After binding gbuffer textures", 95);

	glBindImageTexture(0, lightingOut, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);
	debugging::checkOpenGLErrors("After binding lightingOut texture", 97);

	// dispatch compute shader
	GLuint x = (gbuffer.width + 15) / 16;
	GLuint y = (gbuffer.height + 15) / 16;
	GLuint z = 1;
	glDispatchCompute(x, y, z);
	debugging::checkOpenGLErrors("After dispatching compute shader", 105);

	glMemoryBarrier(GL_FRAMEBUFFER_BARRIER_BIT);

	debugging::checkOpenGLErrors("After getting memory barrier", 109);

	// get it on screen
	glBindFramebuffer(GL_READ_FRAMEBUFFER, lightingFbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	debugging::checkOpenGLErrors("After binding frame buffer", 115);

	// this is bad.
	glBlitFramebuffer(0, 0, gbuffer.width, gbuffer.height, 0, 0, gbuffer.width, gbuffer.height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

	debugging::checkOpenGLErrors("After blit", 120);

	for (int i = 2; i >= 0; --i) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	// done
	glBindVertexArray(0);
	glUseProgram(0);

	debugging::checkOpenGLErrors("After going back to program zero", 130);

}