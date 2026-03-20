#include "Quaternion.h"

Quaternion Quaternion::operator* (Quaternion other)
{
	return other;
}
glm::vec3 Quaternion::operator	* (glm::vec3 vec)
{
	return vec;
}
Quaternion::operator glm::mat4()
{
	return glm::mat4(0);
}
Quaternion Quaternion::conjugate()
{
	return *this;
}