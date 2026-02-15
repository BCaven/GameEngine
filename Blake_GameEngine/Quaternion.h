#pragma once
#include <glm/glm.hpp>

class Quaternion
{
	glm::vec4 quaternion;

public:
	Quaternion operator* (Quaternion other);
	glm::vec3 operator	* (glm::vec3 vec);
	operator glm::mat4();
	Quaternion conjugate();
};

