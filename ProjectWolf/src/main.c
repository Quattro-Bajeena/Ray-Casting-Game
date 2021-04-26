#include "stdafx.h"

#include "utils.h"
#include "sdl_util.h"
#include "Defines.h"
#include "Rendering.h"

#include "Textures.h"
#include "PlayerLogic.h"



int main(void) {

	srand(time(NULL));

	SDL_Window* window3D;
	SDL_Window* windowMap;
	SDL_Renderer* renderer3D;
	SDL_Renderer* rendererMap;


	const Vector2i window3DSize = { 720, 480 };
	const Vector2i windowMapSize = { 720, 720 };

	InitializeSDL(&window3D, &renderer3D, window3DSize.x, window3DSize.y);
	InitializeSDL(&windowMap, &rendererMap, windowMapSize.x, windowMapSize.y);

	SDL_DisplayMode DM;
	SDL_GetCurrentDisplayMode(0, &DM);
	SDL_SetWindowPosition(window3D, DM.w/2 - window3DSize.x, SDL_WINDOWPOS_CENTERED);
	SDL_SetWindowPosition(windowMap, DM.w/2 , SDL_WINDOWPOS_CENTERED);

	InputDir inputDir = { 0,0,0,0 };
	
	Player player = InitPlayer();
	
	float FOV = 60;

	Vector2i mapSize = { 8,8 };

	int tileSize = 64;
	int textureSize = 32;

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

	SDL_SetRenderDrawBlendMode(rendererMap, SDL_BLENDMODE_BLEND);

	SDL_Texture* bufferTexture = SDL_CreateTexture(renderer3D,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		window3DSize.x,
		window3DSize.y);

	SDL_SetTextureBlendMode(bufferTexture, SDL_BLENDMODE_BLEND);

	unsigned char* pixelBuffer = malloc(window3DSize.x * window3DSize.y * 4);
	for (int i = 0; i < window3DSize.x * window3DSize.y; i++) {
		pixelBuffer[(i * 4)] = 0;
		pixelBuffer[(i * 4) + 1] = 0;
		pixelBuffer[(i * 4) + 2] = 0;
		pixelBuffer[(i * 4) + 3] = 0;
	}

	

	//set to 1 when window close button is pressed
	bool running = true;
	while (running) {
		deltaTime = GetDeltaTime(&timeNow, &timeLast);
		//Process events
		SDL_Event sdl_event;
		while (SDL_PollEvent(&sdl_event)) {


			if (sdl_event.type == SDL_WINDOWEVENT) {
				switch (sdl_event.window.event) {
				case SDL_WINDOWEVENT_CLOSE:
					running = false;
					break;
				}
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
		SDL_SetRenderDrawColor(renderer3D, 50, 20, 50, 255); SDL_SetRenderDrawColor(rendererMap, 100, 100, 100, 255);
		SDL_RenderClear(renderer3D); SDL_RenderClear(rendererMap);
		ClearBuffer(pixelBuffer, window3DSize);

	
		
		DrawMap(mapSize, tileSize, mapWalls, rendererMap);
		DrawRays(player, FOV, mapWalls, mapFloor, mapCeiling, mapSize, tileSize, textureSize, TextureAtlas, rendererMap, pixelBuffer, window3DSize);
		DrawPlayer(player, rendererMap);
		

		SDL_UpdateTexture(bufferTexture, NULL, pixelBuffer, window3DSize.x * 4);
		SDL_RenderCopy(renderer3D, bufferTexture, NULL, NULL);
		// draw the image to the window
		
		SDL_RenderPresent(rendererMap);
		SDL_RenderPresent(renderer3D);

	}


	//Clean up resources before exiting
	SDL_DestroyRenderer(renderer3D); SDL_DestroyRenderer(rendererMap);
	SDL_DestroyWindow(window3D); SDL_DestroyWindow(windowMap);
	SDL_Quit();
	return 0;
}