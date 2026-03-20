#include "GameObject.h"

GameObject::GameObject()
{
	bCanTick = false;
	bDynamic = false;
	bVisible = true;
	position = glm::vec3(0, 0, 0);
	scale = glm::vec3(1, 1, 1);
	linearV = glm::vec3(0, 0, 0);
	angularV = glm::vec3(0, 0, 0);
	rotation = Quaternion();
	renderElement = -1;
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

GameObject::GameObject(std::string filePath) : GameObject()
{
	this->filePath = filePath;
	shape = Shape::fromFile(filePath);
}

GameObject::~GameObject()
{
}

glm::mat4 GameObject::getModel()
{
	//glm::mat4 model_matrix = glm::translate(glm::rotate(glm::scale(
	//	glm::mat4(1.0f), scaling), rotation_angle, rotation_axis), translation);
	// translation * rotation * scale
	glm::mat4 rotationMat = glm::mat4(1);
	glm::mat4 model = glm::translate(rotationMat * glm::scale(glm::mat4(1.0f), scale), position);
	return model;
}