#pragma once


void InitializeSDL(SDL_Window** win, SDL_Renderer** renderer, int width, int height);

typedef struct {
	int up, down, left, right;
}InputDir;

typedef struct {
	bool interact;
} Actions;

void GetInput(SDL_Event sdl_event, InputDir* inputDir, Actions* playerActions);

typedef struct {
	int x, y;
}Vector2i;

typedef struct {
	float x, y;
}Vector2f;

double GetDeltaTime(Uint64* timeNow, Uint64* timeLast);