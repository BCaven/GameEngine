#pragma once
//#include "Utility.h"
#include "Debug_Functions.h"

class Texture
{
	
	std::shared_ptr<spdlog::logger> logger;
	GLuint tex;
	rendering::TextureFlags texFlags;
public:
	Texture() : Texture("assets/default_texture.jpg") {}
	Texture(std::string filepath);
	~Texture()
	{

	}
	bool load(unsigned char* data, int width, int height, int channels, GLuint& texture, rendering::TextureFlags flags);
	bool loadFromFile(std::string filePath, GLuint& texture, rendering::TextureFlags flags);
	GLuint get() const { return tex; }
};

