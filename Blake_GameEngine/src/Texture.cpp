#include "Texture.h"

Texture::Texture(std::string filepath)
{
	logger = spdlog::get("shape");
	if (!logger)
	{
		logger = spdlog::stdout_color_mt("shape");
	}

	if (!loadFromFile(filepath, tex, texFlags))
	{
		logger->error("Failed to load texture from file: {}", filepath);
	}
}


bool Texture::load(unsigned char* data, int width, int height, int channels, GLuint& texture, rendering::TextureFlags flags)
{
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	// parse flags
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, flags.TextureWrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, flags.TextureWrapT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, flags.TextureMinFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, flags.TextureMagFilter);

	if (!data)
	{
		logger->warn("Failed to load data when trying to load texture!");
		return false;
	}
	// TODO: support for non-RGB textures
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	return true;
}

bool Texture::loadFromFile(std::string filePath, GLuint& texture, rendering::TextureFlags flags)
{
	int width, height, channels;
	unsigned char* data = nullptr;
	if (!helper::loadImageFile(filePath, width, height, channels, data))
	{
		return false;
	}
	bool r = load(data, width, height, channels, texture, flags);
	helper::freeImage(data);
	return r;
}