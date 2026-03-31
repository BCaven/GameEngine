#pragma once
#include <glm/glm.hpp>
#include <numbers>
#include <cmath>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include "WorldDefaults.h"
#include "Debug_Functions.h"

/*
Inherit vec4 for convenience
going to have some undefined behavior from parent functions but it should be fine.
*/
class Quaternion : public glm::vec4
{
	// < a, b, c, d > = a + bi + cj + dk
	// < x, y, z, w >
	// w + xi + yj + zk
	// even though they are stored with the real part last internally
	// this needs to be remembered when adding
	// TBD override the aliases so they are stored as <w, x, y, z>

public:
	Quaternion() : glm::vec4(0, 0, 0, 1) {}
	Quaternion(float x, float y, float z, float w) : glm::vec4(x, y, z, w) {}
	Quaternion(glm::vec4 vec) : glm::vec4(vec) {}
	Quaternion(glm::vec3 euler);
	Quaternion operator* (Quaternion other);
	Quaternion& operator *= (Quaternion other);
	glm::vec3 operator* (glm::vec3 vec);
	operator glm::mat4();
	Quaternion conjugate();
	glm::vec3 toEuler();

	static Quaternion lookAt(glm::vec3 source, glm::vec3 dest);
	static Quaternion createFromAxisAngle(glm::vec3 axis, float angle);

};

