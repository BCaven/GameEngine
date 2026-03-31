#pragma once
#include <glm/glm.hpp>
#include "GameObject.h"
#include "Utility.h"
#include "WorldDefaults.h"

/*
	Right now, the default camera is a tracking look-at camera.

	TODO: decide if default should be a lookat camera or a camera with a rotation.
*/
class Camera : 
	public GameObject
{
	glm::mat4 projection_matrix;
	glm::vec3 camera_lookat;
	glm::vec3 camera_up;
	glm::vec3 camera_forward;
	float camera_speed = 10;

public:

	Camera();
	Camera(std::string filePath);
	Camera(glm::mat4 proj_mat, glm::vec3 pos, glm::vec3 lookat, glm::vec3 up_vec) : 
		projection_matrix(proj_mat), camera_lookat(lookat), camera_up(up_vec)
	{
		camera_forward = glm::normalize(camera_lookat - position);
	}

	void Tick(float deltaTime) override;

	glm::mat4 BuildCameraMatrix();

};

