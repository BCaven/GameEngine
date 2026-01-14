#include "SDL_Manager.h"

void SDL_Manager::closeWindow(std::uint32_t windowId)
{
	for (int i = 0; i < count; i++)
	{
		if (windowId == SDL_GetWindowID(windows[0]))
		{
			SDL_GL_DestroyContext(opengl_context);
			// TODO: make a quit macro
			for (auto window : windows)
			{
				SDL_DestroyWindow(window);
			}
			count = 0;
			return;

		}
		if (windowId == SDL_GetWindowID(windows[i]))
		{
			SDL_DestroyWindow(windows[i]);
			windows[i] = windows[count - 1];
			count--;
			return;
		}
	}
	logger->warn("Failed to close the window!");
}

void SDL_Manager::spawnWindow(const std::string name, int width, int height, bool resizable)
{
	if (count >= MAX_WINDOWS - 1)
	{
		logger->warn("Already have maximum numbers of windows open");
		return;
	}
	auto flags = SDL_WINDOW_INPUT_FOCUS;
	if (resizable)
	{
		flags |= SDL_WINDOW_RESIZABLE;
	}
	if (count == 0)
	{
		// opengl window gets extra flags
		flags |= SDL_WINDOW_OPENGL;
	}
	windows[count] = SDL_CreateWindow(name.c_str(), width, height, flags);
	if (!windows[count])
	{
		const char* error = SDL_GetError();
		logger->warn("Failed to make the window: {}", error);
		return;
	}

	if (count == 0)
	{
		opengl_context = SDL_GL_CreateContext(windows[count]);
		if (!opengl_context)
		{
			// TOOD: make an error macro
			logger->error("Failed to make gl context!");
			return;
		}
		glewExperimental = GL_TRUE;
		GLenum glewerror = glewInit();
		if (glewerror != GLEW_OK)
		{
			logger->error("Oh no, GLEW threw an error when starting");
		}
		glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
	}
	else
	{


		buffers[count] = SDL_GetWindowSurface(windows[count]);
		if (!buffers[count])
		{
			const char* error = SDL_GetError();
			logger->warn("Failed to make the window surface: {}", error);
			return;
		}
		auto pixelFormat = SDL_GetPixelFormatDetails(buffers[count]->format);
		bool r = SDL_FillSurfaceRect(buffers[count], NULL, SDL_MapRGB(pixelFormat, NULL, 0xff, 0x00, 0xff));
		if (!r)
		{
			const char* error = SDL_GetError();
			logger->warn("Failed to make the window surface: {}", error);
			return;
		}
	}
	logger->debug("Added a new window: {}", count);
	count += 1;

}

void SDL_Manager::updateWindows()
{
	if (windows[0])
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		SDL_GL_SwapWindow(windows[0]);
	}
	for (int i = 1; i < count; i++)
	{
		bool r = SDL_UpdateWindowSurface(windows[i]);
		if (!r)
		{
			const char* error = SDL_GetError();
			logger->warn("Failed to update surface {} with error: {}", i, error);
		}
	}
}

SDL_Manager::SDL_Manager()
{
	SDL_Init(SDL_INIT_VIDEO);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	count = 0;
	opengl_context = NULL;
	for (int i = 0; i < MAX_WINDOWS; i++)
	{
		windows[i] = NULL;
		buffers[i] = NULL;
	}
}
SDL_Manager::~SDL_Manager()
{
	for (auto window : windows)
	{
		SDL_DestroyWindow(window);
	}
	SDL_Quit();
}