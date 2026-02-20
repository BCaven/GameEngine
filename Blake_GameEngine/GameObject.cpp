#include "GameObject.h"

GameObject::GameObject()
{
	bCanTick = false;
	bDynamic = false;
	bVisible = true;
	position = glm::vec3(0, 0, 0);
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

GameObject::GameObject(std::string filePath) : GameObject()
{
	this->filePath = filePath;
	shape = Shape::fromFile(filePath);
}

GameObject::~GameObject()
{
}