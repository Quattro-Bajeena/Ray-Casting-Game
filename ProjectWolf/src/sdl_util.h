#pragma once


void InitializeSDL(SDL_Window** win, SDL_Renderer** renderer, int width, int height);

typedef struct InputDir {
	int up, down, left, right;
}InputDir;

typedef struct Actions {
	bool interact;
} Actions;

void GetInput(SDL_Event sdl_event, InputDir* inputDir, Actions* playerActions);

typedef struct Vector2i {
	int x, y;
}Vector2i;

typedef struct Vector2f {
	float x, y;
}Vector2f;

double GetDeltaTime(Uint64* timeNow, Uint64* timeLast);