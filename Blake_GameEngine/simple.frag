#version 410
smooth in vec3 normal;
smooth in vec3 lightdir;

out vec4 color;
void main()
{

	vec4 default_color = vec4((normal.xyz / 0.5) + 1, 1.0f);
	//vec3 lightdir = vec3(1, 0, 0);
	vec4 diffuse = dot(normal.xyz, lightdir) * default_color;
	//vec4 diffuse = vec4(0.6, 0.6, 0.6, 1.0f);
	color = diffuse;
}