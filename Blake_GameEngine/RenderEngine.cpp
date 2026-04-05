#include "RenderEngine.h"

RenderEngine::RenderEngine()
{
	vert = -1;
	frag = -1;
	program = -1;
	uniformIndexMPV = -1;
	uniformIndexLightDir = -1;
	uniformIndexObjTransform = -1;
	FrameHistory = 1;
	
	logger = spdlog::get("render_engine");
	if (!logger)
	{
		logger = spdlog::stdout_color_mt("render_engine");
	}
}

RenderEngine::~RenderEngine()
{
	glDeleteProgram(program);
	program = -1;
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

bool RenderEngine::loadShader(std::string shaderFile, GLuint shader)
{
	std::string shaderSource = "";
	if (!helper::loadFile(shaderFile, shaderSource))
	{
		spdlog::error("Failed to load vertex shader file: \n{}", shaderFile);
		return false;
	}
	spdlog::info("Loaded vertex shader: \n{}", shaderSource);
	const char* shaderChar = shaderSource.c_str();
	glShaderSource(shader, 1, &shaderChar, NULL);
	glCompileShader(shader);

	int success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		spdlog::error("shader compilation failed: {}", infoLog);
		return false;
	}
	return true;
}

void RenderEngine::loadShaders(std::string shaderName)
{
	vert = glCreateShader(GL_VERTEX_SHADER);
	std::string vertShaderFile = shaderName + ".vert";
	loadShader(vertShaderFile, vert);

	frag = glCreateShader(GL_FRAGMENT_SHADER);
	std::string fragShaderFile = shaderName + ".frag";
	loadShader(fragShaderFile, frag);

	program = glCreateProgram();
	glAttachShader(program, vert);
	glAttachShader(program, frag);
	glLinkProgram(program);

	int success;
	char infoLog[512];
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		logger->error("Shader program linking failed: {}", infoLog);
		// error macro
	}
	glDeleteShader(vert);
	glDeleteShader(frag);
	glUseProgram(0);

	// TODO: better way to handle this
	uniformIndexMPV = glGetUniformLocation(program, "mpv");
	uniformIndexLightDir = glGetUniformLocation(program, "lightDirection");
	uniformIndexObjTransform = glGetUniformLocation(program, "objTransform");

}

void RenderEngine::initialize(std::string shaderName, std::shared_ptr<SceneGraph> SceneGraph_ptr)
{
	loadShaders(shaderName);
	glEnable(GL_CULL_FACE); // don't show faces not pointing towards the camera
	//glCullFace(GL_BACK);
	//glFrontFace(GL_CCW);     
	glEnable(GL_DEPTH_TEST); // show faces with correct depth

	// prep torch tensors
	// TODO: the screen size :)
	// TODO: do we want to keep track of multiple prev frames?
	screen_buffer = torch::zeros({ FrameHistory, 1000, 500, 4 });

	sceneGraph = SceneGraph_ptr;
}

void RenderEngine::RenderFrame(double Delta)
{
	// TODO: update the RenderQueue from the SceneGraph
	RenderQueue = sceneGraph->GetVisibleObjects();

	glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(program);
	// TODO: multiple lights
	glm::vec3 light_direction(std::sin(Delta), 0.5, std::cos(Delta));
	
	glm::mat4 view_projection = RenderCamera->BuildCameraMatrix();
	
	glUniform3fv(uniformIndexLightDir, 1, glm::value_ptr(light_direction));
	glUniformMatrix4fv(uniformIndexMPV, 1, GL_FALSE, glm::value_ptr(view_projection));

	for (std::shared_ptr<GameObject> gameObj : RenderQueue) {
		auto shape = gameObj->getShape();
		glBindVertexArray(shape->getVAO());
	

		glm::mat4 model_matrix = gameObj->getModel();
		glUniformMatrix4fv(uniformIndexObjTransform, 1, GL_FALSE, glm::value_ptr(model_matrix));
	
		glDrawArrays(GL_TRIANGLES, 0, shape->getVertexCount());
	}
	glBindVertexArray(0);
	glUseProgram(0);
}