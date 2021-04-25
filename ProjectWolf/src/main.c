#include "stdafx.h"

#include "utils.h"
#include "sdl_util.h"
#include "Defines.h"
#include "Rendering.h"

#include "Textures.h"
#include "PlayerLogic.h"



int main(void) {

	SDL_Window* win;
	SDL_Renderer* renderer;
	InitializeSDL(&win, &renderer, WINDOW_WIDTH, WINDOW_HEIGHT);

	InputDir inputDir = { 0,0,0,0 };
	
	Player player;
	player.pos.x = 300;	player.pos.y = 300;
	player.angle = 0;
	player.speed = 12;
	player.dir.x = cos(player.angle); 	player.dir.y = sin(player.angle);

	Vector2i mapSize = { 8,8 };

	int tileSize = 64;


	int mapWalls[] = {
		1,1,1,1,1,2,1,1,
		1,0,3,0,0,0,0,1,
		1,0,3,0,0,0,0,1,
		1,0,0,0,0,0,0,1,
		1,1,0,0,0,0,0,1,
		1,0,0,0,1,3,3,3,
		1,0,0,0,4,0,0,1,
		1,1,3,3,3,1,1,1
	};

	int mapFloor[] = {
		1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1
	};

	int mapCeiling[] = {
		2,2,2,2,2,2,2,2,
		2,2,2,2,2,2,2,2,
		2,2,2,2,2,2,2,2,
		2,2,2,2,2,2,2,2,
		2,2,2,2,2,2,2,2,
		2,2,2,2,2,2,2,2,
		2,2,2,2,2,2,2,2,
		2,2,2,2,2,2,2,2
	};


	float fFOV = M_PI / 4.f;	

	Uint64 timeNow = SDL_GetPerformanceCounter();
	Uint64 timeLast = 0;
	double deltaTime = 0;



	//set to 1 when window close button is pressed
	bool running = true;
	
	while (running) {
		deltaTime = GetDeltaTime(&timeNow, &timeLast);

		//Process events
		SDL_Event sdl_event;
		while (SDL_PollEvent(&sdl_event)) {

			switch (sdl_event.type)
			{
			case SDL_QUIT:
				running = false;
				break;
			
			default:
				break;
			}

			GetInput(sdl_event, &inputDir, &player.actions);
		}
		//MOUSE
		int mouse_x, mouse_y;
		int buttons = SDL_GetMouseState(&mouse_x, &mouse_y);

		PlayerMovement(&player, inputDir, mapWalls, tileSize, mapSize, deltaTime);
		PlayerInteraction(&player, mapWalls, tileSize, mapSize);
		
		
		//GAME LOGIC

		//RENDERING
		//clear the window
		SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
		SDL_RenderClear(renderer);

		
		DrawMap(mapSize, tileSize, mapWalls, renderer);
		DrawPlayer(&player, renderer);
		DrawRays(&player, mapWalls,mapFloor, mapCeiling, mapSize, tileSize, TextureAtlas, renderer);
		


		// draw the image to the window
		SDL_RenderPresent(renderer);

	}


	//Clean up resources before exiting
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(win);
	SDL_Quit();
	return 0;
}