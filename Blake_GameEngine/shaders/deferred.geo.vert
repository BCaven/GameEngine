#version 430
layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 norm;
layout(location = 2) in vec2 uv;
uniform mat4 mpv;
uniform mat4 objTransform;
smooth out vec3 normal;
smooth out vec2 uvCoord;


void main()
{
	normal = mat3(objTransform) * norm;
	uvCoord = uv;
	gl_Position = mpv * objTransform * vec4(pos.xyz, 1.0);
}