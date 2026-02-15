#pragma once
#include <chrono>
#include <SDL3/SDL_timer.h>

class Utility
{
private:
	inline static Uint64 StartTimeMillis = SDL_GetTicks();
public:
	static Uint64 getTimeMillis();

	static double getTimeSeconds();

};

