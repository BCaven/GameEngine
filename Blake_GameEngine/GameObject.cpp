#include "GameObject.h"

GameObject::GameObject(std::string filePath, glm::vec3 pos, glm::vec3 s, Quaternion rot)
{
	bCanTick = false;
	bDynamic = false;
	bVisible = true;
	position = pos;
	scale = s;
	linearV = glm::vec3(0, 0, 0);
	angularV = glm::vec3(0, 0, 0);
	rotation = rot;
	renderElement = -1;

	this->filePath = filePath;
	shape = Shape::fromFile(filePath);

	logger = spdlog::get("game_object");
	if (!logger)
	{
		logger = spdlog::stdout_color_mt("game_object");
	}
}

void GameObject::Tick(float deltaTime)
{
	return;
}

GameObject::~GameObject()
{
}

glm::mat4 GameObject::getModel()
{
	glm::mat4 rotationMat = rotation;
	glm::mat4 model = glm::translate(glm::mat4(1), position) * rotationMat * glm::scale(glm::mat4(1.0f), scale);
	return model;
}