#include "stdafx.h"
#include "sdl_util.h"

void InitializeSDL(SDL_Window** win, SDL_Renderer** renderer, int width, int height) {
	//attempt to initizlie graphics system
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		printf("error initializing SDL: %s\n", SDL_GetError());
		return 1;
	}

	(*win) = SDL_CreateWindow("3D Window",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		width, height, 0);

	if ((*win) == NULL) {
		printf("error creating window: %s\n", SDL_GetError());
		SDL_Quit();
		return 1;
	}

	//Create a renderer, which sets up the graphivs hardware
	Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
	 (*renderer) = SDL_CreateRenderer(*win, -1, render_flags);

	if ((*renderer) == NULL) {
		printf("error createing renderer: %s", SDL_GetError());
		SDL_DestroyWindow(*win);
		SDL_Quit();
		return 1;
	}
}

void GetInput(SDL_Event sdl_event, InputDir* inputDir, Actions* playerActions)
{
	switch (sdl_event.type)
	{
	case SDL_KEYDOWN:
		switch (sdl_event.key.keysym.scancode)
		{
		case SDL_SCANCODE_W:
		case SDL_SCANCODE_UP:
			(*inputDir).up = 1;
			break;
		case SDL_SCANCODE_A:
		case SDL_SCANCODE_LEFT:
			(*inputDir).left = 1;
			break;
		case SDL_SCANCODE_S:
		case SDL_SCANCODE_DOWN:
			(*inputDir).down = 1;
			break;
		case SDL_SCANCODE_D:
		case SDL_SCANCODE_RIGHT:
			(*inputDir).right = 1;
			break;
		case SDL_SCANCODE_E:
			(*playerActions).interact = true;
			break;

		default:
			break;
		}
		break;
	case SDL_KEYUP:
		switch (sdl_event.key.keysym.scancode)
		{
		case SDL_SCANCODE_W:
		case SDL_SCANCODE_UP:
			(*inputDir).up = 0;
			break;
		case SDL_SCANCODE_A:
		case SDL_SCANCODE_LEFT:
			(*inputDir).left = 0;
			break;
		case SDL_SCANCODE_S:
		case SDL_SCANCODE_DOWN:
			(*inputDir).down = 0;
			break;
		case SDL_SCANCODE_D:
		case SDL_SCANCODE_RIGHT:
			(*inputDir).right = 0;
			break;
		case SDL_SCANCODE_E:
			(*playerActions).interact = false;
			break;

		default:
			break;
		}
		break;

	default:
		break;
	}

}

double GetDeltaTime(Uint64* timeNow, Uint64* timeLast) {
	*timeLast = *timeNow;
	*timeNow = SDL_GetPerformanceCounter();

	return (double)((*timeNow - *timeLast) / (double)SDL_GetPerformanceFrequency());
}
