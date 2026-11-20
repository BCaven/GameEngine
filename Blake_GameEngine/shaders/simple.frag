#version 410
smooth in vec3 normal;
smooth in vec3 lightdir;
smooth in vec2 uvCoord;

uniform bool bTex;
uniform sampler2D inTexture;

out vec4 color;
void main()
{

	vec4 default_color = vec4((uvCoord.xy / 0.5) + 1, 0.0f, 1.0f);
	//vec3 lightdir = vec3(1, 0, 0);
	float lightInfluence = dot(normal.xyz, lightdir);
	vec4 diffuse = lightInfluence * default_color;
	if (bTex)
	{
		vec4 texColor = texture(inTexture, uvCoord);
		diffuse = texColor * lightInfluence;
	}

	color = diffuse;
}