#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <variant>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include "Debug_Functions.h"


class Shader
{
	GLuint vert;
	GLuint frag;
	GLuint programID;

	std::shared_ptr<spdlog::logger> logger;

	// TODO:
	// Geometry shaders
	// tessellation shaders


	bool loadShader(std::string shaderFile, GLuint shader);
	void loadShaders(std::string shaderName);

	// TODO: I think this should be weak_ptrs so they get automatically updated and we only need to assign their references once
	// TODO: we will also need to have flags if these change per frame and per object

	/*
	Have 3 main types of inputs:
	- constants (things that change rarely but are sill technically dynamic)
	- per frame
	- per object

	For now, I think we are going to have exclusively custom shaders, and this class will be fleshed out later
	*/
	using InputType = std::variant<int, float, glm::vec2, glm::vec3, glm::vec4, glm::mat4>;
	std::vector<std::pair<GLuint, InputType>> inputs;

	std::map<std::string, GLuint> nameIndexMap;

	GLuint checkInput(std::string name);



public:
	Shader(std::string filePath);

	~Shader();

	void Use();

	// each render engine is customized to match its needs
	GLuint AddInput(std::string name);

	template <typename T>
	void SetInput(std::string name, T value);

	void initShader();


	


};

