#version 410
layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 norm;
uniform mat4 mpv;
uniform vec3 lightDirection;
smooth out vec3 normal;
smooth out vec3 lightdir;

void main()
{


	normal = norm;
	lightdir = lightDirection;
	// TODO: Camera matrix
	gl_Position = mpv * vec4(pos.xyz, 1.0);
}