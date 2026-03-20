#pragma once
#include <glm/glm.hpp>
#include "GameObject.h"

class Camera : GameObject
{
	glm::vec3 lookAtTarget;
	glm::mat4 projection_matrix; // = glm::perspective(1.5f, 20.0f / 9.0f, 0.1f, 1000.0f);
	glm::vec3 camera_position; // (std::sin(Delta / 10)* (camDist), std::cos(Delta / 10)* (camDist), 0.5);
	glm::vec3 camera_lookat; // (0, 0, 1.0f);
	glm::vec3 camera_up; // (0, 0.0f, 1.0f);


public:

	Camera();
	Camera(std::string filePath);

	void Tick(float deltaTime) override;

	glm::mat4 BuildCameraMatrix();

};

