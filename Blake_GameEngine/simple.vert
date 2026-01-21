#version 410
layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 norm;
uniform mat4 proj;
smooth out vec3 normal;
void main()
{
	normal = norm;
	gl_Position = vec4(pos.xyz + vec3(0.0, 0.0, 2.0), 1.0); //The addition is an offset as we are not using a “camera” matrix yet..
}