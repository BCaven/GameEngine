#version 430
smooth in vec3 normal;
smooth in vec2 uvCoord;

uniform bool bTex;
uniform sampler2D inTexture;

layout(location = 0) out vec4 gAlbedo;
layout(location = 1) out vec3 gNormal;

void main()
{
	// TODO: material attributes and stuff that isnt albedo and normal maps
	vec4 default_color = vec4((uvCoord.xy / 0.5) + 1, 0.0f, 1.0f);
	vec4 diffuse = default_color;
	if (bTex)
	{
		vec4 texColor = texture(inTexture, uvCoord);
		diffuse = texColor;
	}

	gAlbedo = diffuse;
	gNormal = normalize(normal);
}