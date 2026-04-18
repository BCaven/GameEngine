#include "RenderEngine.h"

RenderEngine::RenderEngine(std::string shaderName) : ActiveShader(shaderName)
{
	FrameHistory = 1;
	logger = spdlog::get("render_engine");
	if (!logger)
	{
		logger = spdlog::stdout_color_mt("render_engine");
	}
}

RenderEngine::~RenderEngine()
{
	// check if anything needs to be explicitly freed
}

bool RenderEngine::loadTextureFromFile(std::string filePath, GLuint& texture, rendering::TextureFlags flags)
{
	int width, height, channels;
	unsigned char* data = nullptr;
	if (!helper::loadImageFile(filePath, width, height, channels, data))
	{
		return false;
	}
	bool r =  loadTexture(data, width, height, channels, texture, flags);
	helper::freeImage(data);
	return r;
}

bool RenderEngine::loadTexture(unsigned char* data, int width, int height, int channels, GLuint& texture, rendering::TextureFlags flags)
{
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	// parse flags
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, flags.TextureWrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, flags.TextureWrapT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, flags.TextureMinFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, flags.TextureMagFilter);

	if (!data)
	{
		logger->warn("Failed to load data when trying to load texture!");
		return false;
	}
	// TODO: support for non-RGB textures
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	return true;
}

void RenderEngine::initialize(std::shared_ptr<SceneGraph> SceneGraph_ptr)
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

void RenderEngine::RenderFrame(double Delta)
{
	// TODO: update the RenderQueue from the SceneGraph
	RenderQueue = sceneGraph->GetVisibleObjects();

	glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	ActiveShader.Use();
	// TODO: multiple lights
	glm::vec3 light_direction(std::sin(Delta), 0.5, std::cos(Delta));
	
	glm::mat4 view_projection = RenderCamera->BuildCameraMatrix();
	
	ActiveShader.SetInput("lightDirection", light_direction);
	ActiveShader.SetInput("mpv", view_projection);

	for (std::shared_ptr<GameObject> gameObj : RenderQueue) {
		auto shape = gameObj->getShape();
		glBindVertexArray(shape->getVAO());
		// bind texture if we have it
		shape->bindTex();

		glm::mat4 model_matrix = gameObj->getModel();
		ActiveShader.SetInput("objTransform", model_matrix);
	
		glDrawArrays(GL_TRIANGLES, 0, shape->getVertexCount());
	}
	glBindVertexArray(0);
	glUseProgram(0);
}