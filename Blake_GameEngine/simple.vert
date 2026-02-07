#version 410
layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 norm;
uniform mat4 proj;
smooth out vec3 normal;
void main()
{
	normal = norm;

	// TODO: Camera matrix
	gl_Position = proj * vec4(pos.xyz, 1.0);
}