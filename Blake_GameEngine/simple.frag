#version 410
smooth in vec3 normal;
out vec4 color;
void main()
{
	vec4 diffuse = vec4(normal.xyz, 1.0f);
	color = diffuse;
}