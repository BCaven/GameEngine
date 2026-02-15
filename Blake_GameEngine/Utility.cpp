#include "Utility.h"

Uint64 Utility::getTimeMillis()
{
	return SDL_GetTicks() - StartTimeMillis;
}

double Utility::getTimeSeconds()
{
	return getTimeMillis() * 0.001;
}