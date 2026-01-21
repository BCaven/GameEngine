#include "SDL_Manager.h"
#include "Shape.h"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

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
		spdlog::error("Failed to load vertex shader file: {}", vertShaderFile);
		return;
	}
	spdlog::info("Loaded vertex shader: {}", vertShader);
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
		spdlog::error("Failed to load fragment shader file: {}", fragShaderFile);
		return;
	}
	spdlog::info("Loaded fragment shader: {}", fragShader);
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
	sdl.spawnWindow("Test Window 2", 500, 500, false);

	loadShaders("simple");

	// test building shapes:
	logger->info("Creating shape from static data");
	Shape StaticShape(1, { 0.0, 0.0, 0.0, 0.5, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0 });
	logger->info("Creating shape from file");
	auto ShapeFromFile = Shape::fromFile("suzanne.bcf");

	if (!ShapeFromFile)
	{
		logger->warn("Failed to read shape from file!");
	}


	SDL_Event e;
	bool quit = false;
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

		if (debugging::checkOpenGLErrors())
		{
			quit = true;
		}

		sdl.updateWindows();
	}
	return 0;
}