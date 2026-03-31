#include "Quaternion.h"

Quaternion::Quaternion(glm::vec3 euler)
{
	// https://www.euclideanspace.com/maths/geometry/rotations/conversions/eulerToQuaternion/index.htm
	// convert Euler to quat
	// assumes angles are in radians
	// for my sanity:
	float& heading = euler.x;
	float& attitude = euler.y;
	float& bank = euler.z;
	float c1 = cos(heading / 2);
	float c2 = cos(attitude / 2);
	float c3 = cos(bank / 2);
	float s1 = sin(heading / 2);
	float s2 = sin(attitude / 2);
	float s3 = sin(bank / 2);
	glm::vec4 norm = glm::normalize(glm::vec4(
		(s1 * s2 * c1) + (c1 * c2 * s3),
		(s1 * c2 * c3) + (c1 * s2 * s3),
		(c1 * s2 * c3) - (s1 * c2 * s3),
		(c1 * c2 * c3) - (s1 * s2 * s3)
	));
	
	w = norm.w;
	x = norm.x;
	y = norm.y;
	z = norm.z;

}

Quaternion Quaternion::operator* (Quaternion other)
{
	// <x, y, z, w> -> w + xi + yj + zk
	// w = w * o.w - x * o.x - y * o.y - z * o.z
	// x = x * o.w + w * o.x - z * o.y + y * o.z
	// y = y * o.w + z * o.x + w * o.y - x * o.z
	// z = z * o.w - y * o.x + x * o.y + w * o.z
	auto& o = other;
	Quaternion prod = Quaternion(
		x * o.w + w * o.x - z * o.y + y * o.z,	// x
		y * o.w + z * o.x + w * o.y - x * o.z,	// y
		z * o.w - y * o.x + x * o.y + w * o.z,	// z
		w * o.w - x * o.x - y * o.y - z * o.z	// w
	);
	return prod;
}

Quaternion& Quaternion::operator *= (Quaternion other)
{
	auto& o = other;
	auto newx = x * o.w + w * o.x - z * o.y + y * o.z;	// x
	auto newy = y * o.w + z * o.x + w * o.y - x * o.z;	// y
	auto newz = z * o.w - y * o.x + x * o.y + w * o.z;	// z
	auto neww = w * o.w - x * o.x - y * o.y - z * o.z;	// w
	x = newx;
	y = newy;
	z = newz;
	w = neww;
	return *this;
}


glm::vec3 Quaternion::operator* (glm::vec3 vec)
{
	// TODO: make sure this quaternion is normalized

	// transform vec3 by a quaternion
	// q * p * q.conjugate()

	// TODO: replace this with the unfolded version
	// and see if there is a meaningful speed difference (since this one is way more readable)

	// TODO: unsure if this is the proper way to convert vec3 -> quat -> vec3
	Quaternion tmp(vec.x, vec.y, vec.z, 1);

	Quaternion r = *this * tmp * conjugate();

	return glm::vec3(r.x, r.y, r.z);
}

Quaternion::operator glm::mat4()
{
	// https://en.wikipedia.org/wiki/Quaternion#Representation_as_real_4_%C3%97_4_matrices
	// https://automaticaddison.com/how-to-convert-a-quaternion-to-a-rotation-matrix/
	float xx = x * x * 2;
	float yy = y * y * 2;
	float zz = z * z * 2;
	float ww = w * w * 2;
	float yz = y * z * 2;
	float wx = x * w * 2;
	float wy = y * w * 2;
	float xz = x * z * 2;
	float wz = z * w * 2;
	float xy = x * y * 2;
	return glm::mat4(
		ww + xx - 1,xy - wz,	xz + wy,	0,
		xy + wz,	ww + yy - 1,yz - wx,	0,
		xz - wy,	yz + wx,	ww + zz - 1,0,
		0,			0,			0,			1
	);
}

/*
Creates a new quaternion conj(q)
*/
Quaternion Quaternion::conjugate()
{
	return Quaternion(-1 * x, -1 * y, -1 * z, w);
}

glm::vec3 Quaternion::toEuler()
{
	// quat does NOT need to be normalized
	// https://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToEuler/
	// this uses w + xi + yj + zk
	// even though that is stored as < x, y, z, w >
	float xx = x * x;
	float yy = y * y;
	float zz = z * z;
	float ww = w * w;
	float unit = xx + yy + zz + ww;
	float test = x * y + z * w;
	// north pole singularity
	if (test > 0.499 * unit)
	{
		float heading = 2 * atan2(x, w);
		float attitude = std::numbers::pi / 2;
		return glm::vec3(heading, attitude, 0);
	}
	// south pole singularity
	if (test < -0.499 * unit)
	{
		float heading = -2 * atan2(x, w);
		float attitude = -1 * std::numbers::pi / 2;
		return glm::vec3(heading, attitude, 0);
	}
	
	// regular case
	float heading = atan2(2 * x * w - 2 * y * z, xx - yy - zz + ww);
	float attitude = asin(2 * test / unit);
	float bank = atan2(2 * x * w - 2 * y * z, (-1 * xx) * yy - zz + ww);
	return glm::vec3(heading, attitude, bank);
}

Quaternion Quaternion::lookAt(glm::vec3 source, glm::vec3 dest)
{
	// https://polylined.com/posts/crafting-lookat-quaternions/
	if (source == dest)
	{
		return Quaternion();
	}
	glm::vec3 lookAtVector = glm::normalize(dest - source);
	glm::vec3 lookAtXZ = glm::normalize(lookAtVector - UP * lookAtVector.y);
	Quaternion quatPitch = Quaternion();
	Quaternion quatYaw = Quaternion();

	if (!helper::approximately(lookAtXZ.x, 0.0f))
	{
		float halfAngleYaw = acos(glm::dot(FORWARD, lookAtXZ)) * 0.5;
		glm::vec3 rotAxis = lookAtVector.x > 0 ? UP : DOWN;
		glm::vec3 quatYawImg = rotAxis * sin(halfAngleYaw);
		quatYaw = Quaternion(quatYawImg.x, quatYawImg.y, quatYawImg.z, cos(halfAngleYaw));
	}
	else if (lookAtXZ.z < 0)
	{
		quatYaw = Quaternion(0, sin(std::numbers::pi * 0.5), 0, cos(std::numbers::pi * 0.5));
	}

	if (!helper::approximately(lookAtVector.y, 0.0f))
	{
		float dotXZ = glm::dot(lookAtXZ, lookAtXZ);
		if (!helper::approximately(dotXZ, 0.0f))
		{
			float halfAnglePitch = acos(glm::dot(lookAtXZ, lookAtVector)) * 0.5;
			glm::vec3 rotAxis = lookAtVector.y < 0 ? RIGHT : LEFT;
			glm::vec3 quatPitchImg = rotAxis * sin(halfAnglePitch);
			quatPitch = Quaternion(quatPitchImg.x, quatPitchImg.y, quatPitchImg.z, cos(halfAnglePitch));
		}
	}
	else
	{
		quatPitch = Quaternion(sin(std::numbers::pi * 0.25), 0, 0, cos(std::numbers::pi * 0.25));
	}
	
	return quatYaw * quatPitch;
}

Quaternion Quaternion::createFromAxisAngle(glm::vec3 axis, float angle)
{
	float halfAngle = angle * 0.5;
	float s = sin(halfAngle);
	return glm::normalize(Quaternion(
		axis.x * s, 
		axis.y * s, 
		axis.z * s, 
		cos(halfAngle)));
}