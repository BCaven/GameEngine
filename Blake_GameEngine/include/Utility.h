#pragma once
#include <chrono>
#include <atomic>
#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_keyboard.h>
#include <SDL3/SDL_scancode.h>
#include <spdlog/sinks/stdout_color_sinks.h>

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