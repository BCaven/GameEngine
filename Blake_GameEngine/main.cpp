#include "SDL_Manager.h"
#include "Shape.h"
#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>
#include <cmath>

GLuint vert;
GLuint frag;
GLuint program;
GLint uniformIndex;

void loadShaders(std::string shaderName)
{
	vert = glCreateShader(GL_VERTEX_SHADER);
	std::string vertShaderFile = shaderName + ".vert";
	std::string vertShader = "";
	if (!helper::loadFile(vertShaderFile, vertShader))
	{
		spdlog::error("Failed to load vertex shader file: \n{}", vertShaderFile);
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
		spdlog::error("Vertex shader compilation failed: {}", infoLog);
		// TODO: error macro
	}

	frag = glCreateShader(GL_FRAGMENT_SHADER);
	std::string fragShaderFile = shaderName + ".frag";
	std::string fragShader = "";
	if (!helper::loadFile(fragShaderFile, fragShader))
	{
		spdlog::error("Failed to load fragment shader file: \n{}", fragShaderFile);
		return;
	}
	spdlog::info("Loaded fragment shader: \n{}", fragShader);
	const char* fragShaderChar = fragShader.c_str();
	glShaderSource(frag, 1, &fragShaderChar, NULL);
	glCompileShader(frag);

	glGetShaderiv(frag, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(frag, 512, NULL, infoLog);
		// TODO: error macro
		spdlog::error("Fragment shader compilation failed: {}", infoLog);
	}
	program = glCreateProgram();
	glAttachShader(program, vert);
	glAttachShader(program, frag);
	glLinkProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		spdlog::error("Shader program linking failed: {}", infoLog);
		// error macro
	}
	glDeleteShader(vert);
	glDeleteShader(frag);
	glUseProgram(0);
	uniformIndex = glGetUniformLocation(program, "proj");

}

int main(int argc, char** argv) 
{
	spdlog::set_level(spdlog::level::info);
	auto logger = spdlog::stdout_color_mt("main");


	SDL_Manager& sdl = SDL_Manager::sdl();

	sdl.spawnWindow("Test Window 1", 500, 500, false);
	//sdl.spawnWindow("Test Window 2", 500, 500, false);

	loadShaders("simple");

	// test building shapes:
	logger->info("Creating shape from static data");
	Shape StaticShape(1, { 
		0.0, 0.0, 0.0, 
		0.5, 1.0, 0.0, 
		1.0, 0.0, 0.0, 
		0.0, 0.0, 1.0, 
		0.0, 0.0, 1.0, 
		0.0, 0.0, 1.0 });
	//logger->info("Creating shape from file");
	auto ShapeFromFile = Shape::fromFile("cube.bcf");

	if (!ShapeFromFile)
	{
		logger->warn("Failed to read shape from file");
		return -1;
	}

	SDL_Event e;
	bool quit = false;
	double frame_num = 0;
	while (!quit)
	{
		while (SDL_PollEvent(&e) != 0)
		{
			switch (e.type)
			{
			case SDL_EVENT_QUIT:
				quit = true;
				break;
			case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
				sdl.closeWindow(e.window.windowID);
				break;

			case SDL_EVENT_WINDOW_RESIZED:
				break;

			}
		}

		// display mesh
		glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBindVertexArray(StaticShape.getVAO());
		glUseProgram(program);
		// ""camera""
		glm::mat4 projection_matrix = glm::perspective(1.5f, 20.0f / 9.0f, 0.1f, 1000.0f);
		glm::vec3 camera_position(std::sin(frame_num), 0.5, std::cos(frame_num));
		glm::vec3 camera_direction(0, 0, 1.0f);
		glm::vec3 camera_up(0, 1.0f, 0);
		glm::mat4 view_matrix = glm::lookAt(camera_position, camera_position + camera_direction, camera_up);
		
		glm::vec3 scaling(1.0f, 1.0f, 1.0f);
		glm::vec3 translation(-0.5f, 0, -0.1f);
		glm::vec3 rotation_axis(1.0f, 0, 0);
		float rotation_angle = 0;
		glm::mat4 model_matrix = glm::translate(glm::rotate(glm::scale(
			glm::mat4(1.0f), scaling), rotation_angle, rotation_axis), translation);
		glm::mat4 model_view_projection = projection_matrix * view_matrix * model_matrix;
		glUniformMatrix4fv(uniformIndex, 1, GL_FALSE, glm::value_ptr(model_view_projection));
		
		if (frame_num == 0)
			logger->info("Drawing shape with {} vertices", StaticShape.getVertexCount());
		glDrawArrays(GL_TRIANGLES, 0, StaticShape.getVertexCount());
		glBindVertexArray(0);
		glUseProgram(0);

		if (debugging::checkOpenGLErrors())
		{
			quit = true;
		}

		sdl.updateWindows();
		frame_num += 0.001;
	}
	return 0;
}