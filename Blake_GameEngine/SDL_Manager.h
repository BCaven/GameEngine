#pragma once

#include <GL/glew.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_opengl.h>

#include <string>
#include <iostream>

#include <spdlog/sinks/stdout_color_sinks.h>

#define MAX_WINDOWS 5

class SDL_Manager
{
private:
	SDL_Manager();
	~SDL_Manager();
	SDL_Manager(const SDL_Manager&) = delete;
	SDL_Manager& operator=(const SDL_Manager&) = delete;

	inline static SDL_Manager* instance = nullptr;
	inline static auto logger = spdlog::stdout_color_mt("sdl_manager");

	SDL_Surface* buffers[MAX_WINDOWS];
	SDL_Window* windows[MAX_WINDOWS];
	SDL_GLContext opengl_context;
	size_t count;


public:
	static SDL_Manager& sdl()
	{
		if (!instance)
		{
			instance = new SDL_Manager();
		}
		return *instance;
	}

	void closeWindow(std::uint32_t windowId);
	void spawnWindow(const std::string name, int width, int height, bool resizable);
	void updateWindows();

};

