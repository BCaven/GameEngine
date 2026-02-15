#include "RenderEngine.h"

RenderEngine::RenderEngine()
{
	vert = -1;
	frag = -1;
	program = -1;
	uniformIndex = -1;
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
	uniformIndex = glGetUniformLocation(program, "proj");

}

void RenderEngine::initialize()
{
	loadShaders("simple");


}

void RenderEngine::RenderFrame(double Delta)
{
	// GRAPHICS: consider putting these in a rendering class.
	glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	for (auto shape : RenderQueue) {
		glBindVertexArray(shape->getVAO());
	}
	glUseProgram(program);

	// TODO: create and use a camera class.
	// ""camera""
	glm::mat4 projection_matrix = glm::perspective(1.5f, 20.0f / 9.0f, 0.1f, 1000.0f);
	glm::vec3 camera_position(std::sin(Utility::getTimeSeconds() / 10) * 5, 0.5, std::cos(Utility::getTimeSeconds() / 10) * 5);
	glm::vec3 camera_lookat(0, 0, 1.0f);
	glm::vec3 camera_up(0, 1.0f, 0);
	glm::mat4 view_matrix = glm::lookAt(camera_position, camera_lookat, camera_up);

	glm::vec3 scaling(1.0f, 1.0f, 1.0f);
	glm::vec3 translation(-0.5f, 0, -0.1f);
	glm::vec3 rotation_axis(1.0f, 0, 0);
	float rotation_angle = 0;
	glm::mat4 model_matrix = glm::translate(glm::rotate(glm::scale(
		glm::mat4(1.0f), scaling), rotation_angle, rotation_axis), translation);
	glm::mat4 model_view_projection = projection_matrix * view_matrix * model_matrix;
	glUniformMatrix4fv(uniformIndex, 1, GL_FALSE, glm::value_ptr(model_view_projection));

	for (auto shape : RenderQueue) {
		glDrawArrays(GL_TRIANGLES, 0, shape->getVertexCount());
	}
	glBindVertexArray(0);
	glUseProgram(0);
}