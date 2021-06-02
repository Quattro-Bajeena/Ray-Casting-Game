#include "stdafx.h"

#include "utils.h"
#include "sdl_util.h"
#include "Defines.h"
#include "Rendering.h"
#include "LoadFiles.h"
#include "PlayerLogic.h"


int main(void) {

	srand(time(NULL));

	Player player = InitPlayer();
	InputDir inputDir = { 0,0,0,0 };

	float FOV = 120;

	int tileSize = 64;
	int textureSize = 64;

	Vector2i mapSize = { 0,0 };
	int* mapWalls = LoadMap("Maps/map1.txt", &mapSize);

	float fFOV = M_PI / 4.f;	

	
	// Initialize graphics
	SDL_Window* window3D;
	SDL_Window* windowMap;
	SDL_Renderer* renderer3D;
	SDL_Renderer* rendererMap;


	const Vector2i window3DSize = { 720, 480 };
	const Vector2i windowMapSize = { mapSize.x * tileSize, mapSize.y * tileSize };

	// Initializing Windows
	InitializeSDL(&window3D, &renderer3D, window3DSize.x, window3DSize.y);
	InitializeSDL(&windowMap, &rendererMap, windowMapSize.x, windowMapSize.y);

	SDL_DisplayMode DM;
	SDL_GetCurrentDisplayMode(0, &DM);
	SDL_SetWindowPosition(window3D, DM.w / 2 - window3DSize.x, SDL_WINDOWPOS_CENTERED);
	SDL_SetWindowPosition(windowMap, DM.w / 2, SDL_WINDOWPOS_CENTERED);

	SDL_SetRenderDrawBlendMode(rendererMap, SDL_BLENDMODE_BLEND);

	// Initializing texture that will be drawn to the 3D window 
	SDL_Texture* bufferTexture = SDL_CreateTexture(renderer3D,
		SDL_PIXELFORMAT_RGBA32,
		SDL_TEXTUREACCESS_STREAMING,
		window3DSize.x,
		window3DSize.y);
	SDL_SetTextureBlendMode(bufferTexture, SDL_BLENDMODE_BLEND);

	// Initializing pixel buffer that the raycasting algorithm will directly modify.
	// It will be copied to bufferTexture before every draw call
	SDL_Color* pixelBuffer = malloc(window3DSize.x * window3DSize.y * sizeof(SDL_Color));
	SDL_Color black = { 0,0,0,255 };
	for (int i = 0; i < window3DSize.x * window3DSize.y; i++) {
		pixelBuffer[i] = black;
	}

	// Initializing wall textures
	const int textureNumber = 3;
	SDL_Surface** surfaces = malloc(textureNumber * sizeof(SDL_Surface*));
	surfaces[0] = IMG_Load("Textures/MultibrickL.png");
	surfaces[1] = IMG_Load("Textures/GrasswallL.png");
	surfaces[2] = IMG_Load("Textures/BluegreyL.png");

	
	SDL_Texture** textures = malloc(textureNumber * sizeof(SDL_Texture*));
	for (int i = 0; i < textureNumber; i++) {
		textures[i] = SDL_CreateTextureFromSurface(rendererMap, surfaces[i]);
	}

	// Variables to track time passed since the last frame
	Uint64 timeNow = SDL_GetPerformanceCounter();
	Uint64 timeLast = 0;
	double deltaTime = 0;

	//set to 1 when window close button is pressed
	bool running = true;
	while (running) {
		// Get time from last frame
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
			// Get player input from KEYDOWN events
			GetInput(sdl_event, &inputDir, &player.actions);
		}
		//MOUSE
		int mouse_x, mouse_y;
		int buttons = SDL_GetMouseState(&mouse_x, &mouse_y);

		//GAME LOGIC
		PlayerMovement(&player, inputDir, mapWalls, tileSize, mapSize, deltaTime);
		PlayerInteraction(&player, mapWalls, tileSize, mapSize);

		//RENDERING
		//clear the window
		SDL_SetRenderDrawColor(renderer3D, 100, 100, 100, 255); SDL_SetRenderDrawColor(rendererMap, 100, 100, 100, 255);
		SDL_RenderClear(renderer3D); SDL_RenderClear(rendererMap);
		ClearBuffer(pixelBuffer, window3DSize);

		// 2D map drawn to rendererMap
		DrawMap(mapSize, tileSize, mapWalls, textures, rendererMap);
		// 3D raycast image drawn to pixelBuffer, rays drawn to map
		DrawRays(player, FOV, mapWalls, mapSize, tileSize, textureSize, surfaces, rendererMap, pixelBuffer, window3DSize);
		// player drawn on map
		DrawPlayer(player, rendererMap);
		
		// pixel data from buffer is copied to renderTexture, so it can be displayed by the renderer
		SDL_UpdateTexture(bufferTexture, NULL, pixelBuffer, window3DSize.x * 4);
		SDL_RenderCopy(renderer3D, bufferTexture, NULL, NULL);

		// draw the final images to the windows
		SDL_RenderPresent(rendererMap);
		SDL_RenderPresent(renderer3D);

	}


	//Clean up resources before exiting
	SDL_DestroyRenderer(renderer3D); SDL_DestroyRenderer(rendererMap);
	SDL_DestroyWindow(window3D); SDL_DestroyWindow(windowMap);
	SDL_Quit();
	return 0;
}