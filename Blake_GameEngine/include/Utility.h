#pragma once
#include <chrono>
#include <atomic>
#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_keyboard.h>
#include <SDL3/SDL_scancode.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include "SDL_Manager.h"

namespace rendering
{
	struct TextureFlags
	{
		GLuint TextureWrapS = GL_REPEAT;
		GLuint TextureWrapT = GL_REPEAT;
		GLuint TextureMinFilter = GL_LINEAR_MIPMAP_LINEAR;
		GLuint TextureMagFilter = GL_LINEAR;
	};

	// TODO: use TextureFlags struct
	inline void makeTex2D(GLsizei width, GLsizei height, GLuint& tex, GLint internalFmt, GLenum fmt, GLenum type, GLenum attach)
	{
		glGenTextures(1, &tex);
		glBindTexture(GL_TEXTURE_2D, tex);
		glTexImage2D(GL_TEXTURE_2D, 0, internalFmt, width, height, 0, fmt, type, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, attach, GL_TEXTURE_2D, tex, 0);
	}

	// Frame Buffer Object
	struct gBuffer {
		GLuint fbo = 0, albedo = 0, normal = 0, depth = 0;
		int width = 0, height = 0;

		void Create(int w, int h)
		{
			width = w;
			height = h;
			glGenFramebuffers(1, &fbo);
			glBindFramebuffer(GL_FRAMEBUFFER, fbo);
			makeTex2D(width, height, albedo, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, GL_COLOR_ATTACHMENT0);
			makeTex2D(width, height, normal, GL_RGB16F, GL_RGB, GL_FLOAT, GL_COLOR_ATTACHMENT1);
			makeTex2D(width, height, depth, GL_DEPTH_COMPONENT32F, GL_DEPTH_COMPONENT, GL_FLOAT, GL_DEPTH_ATTACHMENT);

			GLenum bufs[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
			glDrawBuffers(2, bufs);

			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			{
				spdlog::error("G-Buffer FBO incomplete!");
			}

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		void Destroy()
		{
			glDeleteTextures(1, &albedo);
			glDeleteTextures(1, &normal);
			glDeleteTextures(1, &depth);
			glDeleteFramebuffers(1, &fbo);
		}
	};
}

class Utility
{
private:
	inline static Uint64 StartTimeMillis = SDL_GetTicks();
public:
	static Uint64 getTimeMillis();

	static double getTimeSeconds();

	inline static Uint64 getTotalTime() { return SDL_GetTicks(); }

};

/*
Class structure:
private physical keys to be bound (or maybe just use the SDL versions of these

public atomic booleans for actions we want (e.g. forward, backward, left, right) that the end user references

*/
class KeyInputs
{
private:
	inline static KeyInputs* instance = nullptr;
	KeyInputs();
	~KeyInputs();
	KeyInputs(const KeyInputs&) = delete;
	KeyInputs& operator=(const KeyInputs&) = delete;

	inline static auto logger = spdlog::stdout_color_mt("input");

	static inline SDL_Manager* sdl = nullptr;

	// TODO: constructing this
	std::map<SDL_Keycode, std::atomic<bool>&> keybindings;

	const bool* keyboardState;

	
public:
	
	static KeyInputs& inputHandler(SDL_Manager *sdl_manager)
	{
		if (!instance)
		{
			instance = new KeyInputs();
		}
		sdl = sdl_manager;
		return *instance;
	}

	static void DeleteInstance()
	{
		delete instance;
		instance = nullptr;
	}

	// in a funny sense, it might be fine to make these not atomic since we only have one writer
	
	static inline std::atomic<bool> FORWARD = false;
	static inline std::atomic<bool> BACKWARD = false;
	static inline std::atomic<bool> LEFT = false;
	static inline std::atomic<bool> RIGHT = false;
	static inline std::atomic<float> MOUSE_X = 0.0;
	static inline std::atomic<float> MOUSE_Y = 0.0;
	static inline std::atomic<bool> MOUSE_PRESSED = false;
	static inline std::atomic<bool> QUIT = false;


	void handleKeyInputs();
	void registerKeybindings(std::string filePath);
};