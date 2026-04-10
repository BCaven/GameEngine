#include "Utility.h"

Uint64 Utility::getTimeMillis()
{
	return SDL_GetTicks() - StartTimeMillis;
}

double Utility::getTimeSeconds()
{
	return getTimeMillis() * 0.001;
}

void KeyInputs::handleKeyInputs()
{
	float x, y;
	auto mouse_buttons = SDL_GetMouseState(&x, &y);
	MOUSE_X = x;
	MOUSE_Y = y;

	// TODO: have a better way to do this
	FORWARD = keyboardState[SDL_SCANCODE_W];
	BACKWARD = keyboardState[SDL_SCANCODE_S];
	LEFT = keyboardState[SDL_SCANCODE_A];
	RIGHT = keyboardState[SDL_SCANCODE_D];
	QUIT = keyboardState[SDL_SCANCODE_Q];

	SDL_Event e;
	while (SDL_PollEvent(&e) != 0)
	{
		switch (e.type)
		{
		case SDL_EVENT_QUIT:
			QUIT = true;
			break;
		case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
			sdl->closeWindow(e.window.windowID);
			break;

		case SDL_EVENT_WINDOW_RESIZED:
			break;
		}
	}
}

void KeyInputs::registerKeybindings(std::string filePath)
{
	logger->warn("Registering custom keybinds is not implemented yet");
}

KeyInputs::KeyInputs()
{
	FORWARD = false;
	BACKWARD = false;
	LEFT = false;
	RIGHT = false;
	QUIT = false;
	MOUSE_PRESSED = false;
	MOUSE_X = 0;
	MOUSE_Y = 0;
	keyboardState = SDL_GetKeyboardState(nullptr);
}

KeyInputs::~KeyInputs()
{

}