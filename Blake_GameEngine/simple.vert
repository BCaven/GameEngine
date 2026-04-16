#version 410
layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 norm;
layout(location = 2) in vec2 uv;
uniform mat4 mpv;
uniform mat4 objTransform;
uniform vec3 lightDirection;
smooth out vec3 normal;
smooth out vec3 lightdir;
smooth out vec2 uvCoord;

void main()
{
	normal = norm;
	uvCoord = uv;
	vec4 tmp = vec4(lightDirection.xyz, 1.0);
	lightdir = (tmp * objTransform).xyz;
	gl_Position = mpv * objTransform * vec4(pos.xyz, 1.0);
}