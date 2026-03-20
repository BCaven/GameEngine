#include "Camera.h"

Camera::Camera() : GameObject()
{
	// cameras should default to being invisible.
	bVisible = false;

	projection_matrix = glm::perspective(1.5f, 20.0f / 9.0f, 0.1f, 1000.0f);
	camera_position = glm::vec3(1, 0, 0.5);
	camera_lookat = glm::vec3(0, 0, 1.0f);
	camera_up = glm::vec3(0, 0.0f, 1.0f);
}

Camera::Camera(std::string filePath) : GameObject(filePath)
{
	// not sure what else I want this one to do other than if I make a model for cameras
}

glm::mat4 Camera::BuildCameraMatrix()
{
	// TODO: actual camera
	glm::mat4 view_matrix = glm::lookAt(camera_position, camera_lookat, camera_up);

	glm::mat4 view_projection = projection_matrix * view_matrix;
	return view_projection;
}

void Camera::Tick(float deltaTime)
{

}