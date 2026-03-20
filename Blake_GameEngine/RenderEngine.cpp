#include "RenderEngine.h"

RenderEngine::RenderEngine()
{
	vert = -1;
	frag = -1;
	program = -1;
	uniformIndexMPV = -1;
	
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


void RenderEngine::loadShaders(std::string shaderName)
{
	// TODO: rewrite to handle tessellation shaders
	vert = glCreateShader(GL_VERTEX_SHADER);
	std::string vertShaderFile = shaderName + ".vert";
	std::string vertShader = "";
	if (!helper::loadFile(vertShaderFile, vertShader))
	{
		logger->error("Failed to load vertex shader file: \n{}", vertShaderFile);
		return;
	}
	spdlog::info("Loaded vertex shader: \n{}", vertShader);
	const char* vertShaderChar = vertShader.c_str();
	glShaderSource(vert, 1, &vertShaderChar, NULL);
	glCompileShader(vert);

	int success;
	char infoLog[512];
	glGetShaderiv(vert, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vert, 512, NULL, infoLog);
		logger->error("Vertex shader compilation failed: {}", infoLog);
		// TODO: error macro
	}

	frag = glCreateShader(GL_FRAGMENT_SHADER);
	std::string fragShaderFile = shaderName + ".frag";
	std::string fragShader = "";
	if (!helper::loadFile(fragShaderFile, fragShader))
	{
		logger->error("Failed to load fragment shader file: \n{}", fragShaderFile);
		return;
	}
	logger->info("Loaded fragment shader: \n{}", fragShader);
	const char* fragShaderChar = fragShader.c_str();
	glShaderSource(frag, 1, &fragShaderChar, NULL);
	glCompileShader(frag);

	glGetShaderiv(frag, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(frag, 512, NULL, infoLog);
		// TODO: error macro
		logger->error("Fragment shader compilation failed: {}", infoLog);
	}
	program = glCreateProgram();
	glAttachShader(program, vert);
	glAttachShader(program, frag);
	glLinkProgram(program);

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
	uniformIndexMPV = glGetUniformLocation(program, "mpv");
	uniformIndexLightDir = glGetUniformLocation(program, "lightDirection");
	uniformIndexObjTransform = glGetUniformLocation(program, "objTransform");

}

void RenderEngine::initialize(std::string shaderName, std::shared_ptr<SceneGraph> SceneGraph_ptr)
{
	loadShaders(shaderName);
	glEnable(GL_CULL_FACE); // dont show faces not pointing towards the camera
	//glCullFace(GL_BACK);
	//glFrontFace(GL_CCW);     
	glEnable(GL_DEPTH_TEST); // show faces with correct depth

	sceneGraph = SceneGraph_ptr;
}

void RenderEngine::RenderFrame(double Delta)
{
	// TODO: update the RenderQueue from the SceneGraph
	RenderQueue = sceneGraph->GetVisibleObjects();

	glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(program);
	// TODO: actual camera
	glm::vec3 light_direction(std::sin(Delta), 0.5, std::cos(Delta));
	
	glm::mat4 view_projection = RenderCamera->BuildCameraMatrix();
	
	// TODO: make sure we can actually do this
	glUniform3fv(uniformIndexLightDir, 1, glm::value_ptr(light_direction));
	glUniformMatrix4fv(uniformIndexMPV, 1, GL_FALSE, glm::value_ptr(view_projection));

	for (auto gameObj : RenderQueue) {
		auto shape = gameObj->getShape();
		glBindVertexArray(shape->getVAO());
	

		glm::mat4 model_matrix = gameObj->getModel();
		glUniformMatrix4fv(uniformIndexObjTransform, 1, GL_FALSE, glm::value_ptr(model_matrix));
	
		glDrawArrays(GL_TRIANGLES, 0, shape->getVertexCount());
	}
	glBindVertexArray(0);
	glUseProgram(0);
}