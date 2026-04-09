#include "Shader.h"

Shader::Shader(std::string filePath)
{
	logger = spdlog::get("shader");
	if (!logger)
	{
		logger = spdlog::stdout_color_mt("shader");
	}
	loadShaders(filePath);
}

bool Shader::loadShader(std::string shaderFile, GLuint shader)
{
	std::string shaderSource = "";
	if (!helper::loadFile(shaderFile, shaderSource))
	{
		spdlog::error("Failed to load shader file: \n{}", shaderFile);
		return false;
	}
	spdlog::info("Loaded shader: \n{}", shaderSource);
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

void Shader::loadShaders(std::string shaderName)
{
	// TODO: better way for searching for shaders
	// I like the naming convention but we will need to check for geo and tessl shaders later
	// and I do not want to have a flat directory forever
	vert = glCreateShader(GL_VERTEX_SHADER);
	std::string vertShaderFile = shaderName + ".vert";
	loadShader(vertShaderFile, vert);

	frag = glCreateShader(GL_FRAGMENT_SHADER);
	std::string fragShaderFile = shaderName + ".frag";
	loadShader(fragShaderFile, frag);

	programID = glCreateProgram();
	glAttachShader(programID, vert);
	glAttachShader(programID, frag);
	glLinkProgram(programID);

	int success;
	char infoLog[512];
	glGetProgramiv(programID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(programID, 512, NULL, infoLog);
		logger->error("Shader program linking failed: {}", infoLog);
		// error macro
	}
	glDeleteShader(vert);
	glDeleteShader(frag);
	glUseProgram(0);
}

void Shader::Use()
{
	glUseProgram(programID);
}

void Shader::initShader()
{
	logger->warn("This function has not been written");
}

GLuint Shader::checkInput(std::string name)
{
	auto val = nameIndexMap.find(name);
	if (val == nameIndexMap.end())
	{
		logger->warn("Input {} not found in shader! Registering...", name);
		// auto create input
		return AddInput(name);
	}
	return val->second;
}


GLuint Shader::AddInput(std::string name)
{
	GLuint location = glGetUniformLocation(programID, name.c_str());
	nameIndexMap[name] = location;

	// in a perfect world, these would auto-update
	//inputs.push_back({ location, value });
	return location;
}

template <typename T>
void Shader::SetInput(std::string name, T value)
{
	logger->warn("This type is not supported!");
}
template <>
void Shader::SetInput<int>(std::string name, int value)
{
	GLuint location = checkInput(name);
	glUniform1i(location, value);
}
template <>
void Shader::SetInput<float>(std::string name, float value)
{
	GLuint location = checkInput(name);
	glUniform1f(location, value);
}
template <>
void Shader::SetInput<glm::vec2>(std::string name, glm::vec2 value)
{
	GLuint location = checkInput(name);
	glUniform2fv(location, 1, glm::value_ptr(value));
}
template <>
void Shader::SetInput<glm::vec3>(std::string name, glm::vec3 value)
{
	GLuint location = checkInput(name);
	glUniform3fv(location, 1, glm::value_ptr(value));
}
template <>
void Shader::SetInput<glm::vec4>(std::string name, glm::vec4 value)
{
	GLuint location = checkInput(name);
	glUniform4fv(location, 1, glm::value_ptr(value));
}
template <>
void Shader::SetInput<glm::mat4>(std::string name, glm::mat4 value)
{
	GLuint location = checkInput(name);
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

