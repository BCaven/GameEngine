#include "RenderEngineForward.h"

void ForwardRenderEngine::bindTex(std::shared_ptr<Texture> tex)
{
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

void ForwardRenderEngine::initialize(std::shared_ptr<SceneGraph> SceneGraph_ptr)
{
	// TODO: have all of this happen in the constructor
	glEnable(GL_CULL_FACE); // don't show faces not pointing towards the camera
	//glCullFace(GL_BACK);
	//glFrontFace(GL_CCW);     
	glEnable(GL_DEPTH_TEST); // show faces with correct depth

	// prep torch tensors
	// TODO: the screen size :)
	// TODO: do we want to keep track of multiple prev frames?
	//screen_buffer = torch::zeros({ FrameHistory, 1000, 500, 4 });

	// TODO: need to figure out why these always return -1 as the location


	ActiveShader.AddInput("lightDirection");
	ActiveShader.AddInput("mvp");
	ActiveShader.AddInput("objTransform");

	sceneGraph = SceneGraph_ptr;
}

void ForwardRenderEngine::RenderFrame(double Delta)
{
	logger->info("Running forward RenderFrame function");
	if (!RenderCamera)
	{
		logger->warn("No camera selected!");
		return;
	}
	// TODO: update the RenderQueue from the SceneGraph
	RenderQueue = sceneGraph->GetAllObjects();

	glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	ActiveShader.Use();
	// TODO: multiple lights
	glm::vec3 light_direction(std::sin(Delta), 0.5, std::cos(Delta));
	
	glm::mat4 view_projection = RenderCamera->BuildCameraMatrix();
	
	ActiveShader.SetInput("lightDirection", light_direction);
	ActiveShader.SetInput("mpv", view_projection);

	for (std::shared_ptr<GameObject> gameObj : RenderQueue) {
		if (!gameObj)
		{
			logger->error("Bad pointer to game object!");
			continue;
		}
		auto shape = gameObj->getShape();	
		bindTex(shape->getTexture());
		
		glBindVertexArray(shape->getVAO());
		// bind texture if we have it

		glm::mat4 model_matrix = gameObj->getModel();
		ActiveShader.SetInput("objTransform", model_matrix);
	
		glDrawArrays(GL_TRIANGLES, 0, shape->getVertexCount());
	}
	glBindVertexArray(0);
	glUseProgram(0);
}