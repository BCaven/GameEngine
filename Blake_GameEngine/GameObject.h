#pragma once
#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include "Quaternion.h"
#include "Shape.h"

class GameObject
{
protected:
	bool bCanTick;
	bool bDynamic;
	bool bVisible;
	glm::vec3 position;
	glm::vec3 scale;
	Quaternion rotation;
	int renderElement;

	std::string filePath;
	std::shared_ptr<Shape> shape;

	glm::vec3 linearV; // TBD: make a subclass that can move and have the base class unable to move.
	glm::vec3 angularV;
	std::shared_ptr<spdlog::logger> logger;
	

public:
	/*
	TODO: GameObject file type so we can just auto load them

	TODO: support for multiple shapes in a GameObject
	*/
	GameObject() : GameObject("default.bcf") {}
	~GameObject();
	GameObject(std::string filePath) : GameObject(filePath, glm::vec3(0), glm::vec3(1), glm::vec3(0)) {}
	GameObject(std::string filePath, glm::vec3 pos, glm::vec3 s, glm::vec3 rot) : GameObject(filePath, pos, s, Quaternion(rot)) {}
	GameObject(std::string filePath, glm::vec3 pos, glm::vec3 s, Quaternion rot);

	glm::mat4 getModel();
	int getRenderElement() { return renderElement; }

	virtual void Tick(float deltaTime);
	inline void SetCanTick(bool canTick) { bCanTick = canTick; }
	inline bool CanTick()  { return bCanTick; }
	inline void SetDynamic(bool isDynamic) { bDynamic = isDynamic; }
	inline bool IsDynamic()  { return bDynamic; }
	inline void SetVisible(bool isVisible) { bVisible = isVisible; }
	inline bool IsVisible()  { return bVisible; }
	inline std::shared_ptr<Shape> getShape() { return shape; }
	inline void SetPosition(glm::vec3 newPos) { position = newPos; }
	inline glm::vec3 GetPosition() { return position; }
	inline Quaternion GetRotation() { return rotation; }
};

