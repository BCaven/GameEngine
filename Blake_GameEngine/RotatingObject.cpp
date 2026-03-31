#include "RotatingObject.h"


void RotatingObject::Tick(float deltaTime)
{
	rotation_tick = Quaternion::createFromAxisAngle(rotation_axis, rotation_speed * deltaTime);

	//logger->info("Rotating object is ticking!");
	// rotate around the rotation axis by rot speed
	rotation = rotation * rotation_tick;
	//rotation = glm::normalize(rotation);
	//logger->info("Rotation: {}, {}i, {}j, {}k", rotation.w, rotation.x, rotation.y, rotation.z);
}