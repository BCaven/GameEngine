#include "Camera.h"

Camera::Camera() : GameObject()
{
	// cameras should default to being invisible.
	bVisible = false;
	bCanTick = true;
	projection_matrix = glm::perspective(1.5f, 20.0f / 9.0f, 0.1f, 1000.0f);
	position = glm::vec3(0, 1, 0.5);
	camera_lookat = glm::vec3(0, 0, 0);
	camera_up = UP;
	camera_forward = glm::normalize(camera_lookat - position);

}

Camera::Camera(std::string filePath) : GameObject(filePath)
{
	bVisible = false;
	bCanTick = true;
	projection_matrix = glm::perspective(1.5f, 20.0f / 9.0f, 0.1f, 1000.0f);
	position = glm::vec3(0, 1, 0.5);
	camera_lookat = glm::vec3(0, 0, 0);
	camera_up = UP;
	camera_forward = glm::normalize(camera_lookat - position);

}

glm::mat4 Camera::BuildCameraMatrix()
{
	glm::mat4 view_matrix = glm::lookAt(position, camera_lookat, camera_up);

	glm::mat4 view_projection = projection_matrix * view_matrix;
	return view_projection;
}

void Camera::Tick(float deltaTime)
{
	if (KeyInputs::FORWARD)
	{
		position += camera_forward * camera_speed * deltaTime;

		camera_forward = glm::normalize(camera_lookat - position);
	}
	if (KeyInputs::BACKWARD)
	{
		position -= camera_forward * camera_speed * deltaTime;

		camera_forward = glm::normalize(camera_lookat - position);
	}
	if (KeyInputs::LEFT)
	{
		position -= glm::cross(camera_forward, UP) * camera_speed * deltaTime;

		camera_forward = glm::normalize(camera_lookat - position);
	}
	if (KeyInputs::RIGHT)
	{
		position += glm::cross(camera_forward, UP) * camera_speed * deltaTime;

		camera_forward = glm::normalize(camera_lookat - position);
	}
}