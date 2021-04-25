#include "stdafx.h"

#define WINDOW_HEIGHT (480)
#define WINDOW_WIDTH (640)

//scroll speed in pixels/second
#define SCROLL_SPEED (300)
#define SPEED (300)




int _main(void) {

	//attempt to initizlie graphics system
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		printf("error initializing SDL: %s\n", SDL_GetError());
		return 1;
	}

	SDL_Window* win = SDL_CreateWindow("HEllo",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		WINDOW_WIDTH, WINDOW_HEIGHT, 0);

	if (!win) {
		printf("error creating window: %s\n", SDL_GetError());
		SDL_Quit();
		return 1;
	}

	//Create a renderer, which sets up the graphivs hardware
	Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
	SDL_Renderer* rend = SDL_CreateRenderer(win, -1, render_flags);

	if (!rend) {
		printf("error createing renderer: %s", SDL_GetError());
		SDL_DestroyWindow(win);
		SDL_Quit();
		return 1;
	}

	//load the image into memory using SDL_image lubary function
	SDL_Surface* surface = IMG_Load("resources/azusa.png");
	if (!surface) {
		printf("error creating surface: %s\n", SDL_GetError());
		SDL_DestroyRenderer(rend);
		SDL_DestroyWindow(win);
		SDL_Quit();
		return 1;
	}

	//Load the image data into graphics hardware memory
	SDL_Texture* tex = SDL_CreateTextureFromSurface(rend, surface);
	if (!tex) {
		printf("error creating texture: %s\n", SDL_GetError());
		SDL_DestroyRenderer(rend);
		SDL_DestroyWindow(win);
		SDL_Quit();
		return 1;
	}

	//struct to hold the position and size of the spirte
	SDL_Rect dest;

	//Get the dimension of the texture
	SDL_QueryTexture(tex, NULL, NULL, &dest.w, &dest.h);
	dest.w /= 4;
	dest.h /= 4;

	//position the sprite at the bottom center of the window
	// origin is the top left corner, positive y is down
	dest.x = (WINDOW_WIDTH - dest.w) / 2;
	

	//start spirte in center of screen
	float x_pos = (WINDOW_WIDTH - dest.w) / 2;
	float y_pos = (WINDOW_HEIGHT - dest.h) / 2;
	float x_vel = 0;
	float y_vel = 0;

	int up = 0;
	int down = 0;
	int left = 0;
	int right = 0;
	


	//set to 1 when window close button is pressed
	int close_requested = 0;

	while (!close_requested) {
		//Process events
		SDL_Event sdl_event;
		while (SDL_PollEvent(&sdl_event)) {
			if (sdl_event.type == SDL_QUIT) {
				close_requested = 1;
			}
			switch (sdl_event.type)
			{
			case SDL_QUIT:
				close_requested = 1;
				break;
			case SDL_KEYDOWN:
				switch (sdl_event.key.keysym.scancode)
				{
				case SDL_SCANCODE_W:
				case SDL_SCANCODE_UP:
					up = 1;
					break;
				case SDL_SCANCODE_A:
				case SDL_SCANCODE_LEFT:
					left = 1;
					break;
				case SDL_SCANCODE_S:
				case SDL_SCANCODE_DOWN:
					down = 1;
					break;
				case SDL_SCANCODE_D:
				case SDL_SCANCODE_RIGHT:
					right = 1;
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
					up = 0;
					break;
				case SDL_SCANCODE_A:
				case SDL_SCANCODE_LEFT:
					left = 0;
					break;
				case SDL_SCANCODE_S:
				case SDL_SCANCODE_DOWN:
					down = 0;
					break;
				case SDL_SCANCODE_D:
				case SDL_SCANCODE_RIGHT:
					right = 0;
					break;

				default:
					break;
				}
				break;

			default:
				break;
			}
		}

		int mouse_x, mouse_y;
		int buttons = SDL_GetMouseState(&mouse_x, &mouse_y);
		printf("%i %i\n", mouse_x, mouse_y);

		//determin belocity towards mouse
		int target_x = mouse_x - dest.w / 2;
		int target_y = mouse_y - dest.h / 2;
		float delta_x = target_x - x_pos;
		float delta_y = target_y - y_pos;
		float distance = sqrt(delta_x * delta_x + delta_y * delta_y);

		//prevents jitter
		if (distance < 5) {
			x_vel = y_vel = 0;
		}
		else {
			x_vel = delta_x * SPEED / distance;
			y_vel = delta_y * SPEED / distance;
		}

		//Reverse volicty if mouse button 1 prssed
		if (buttons & SDL_BUTTON(SDL_BUTTON_LEFT)) {
			x_vel *= -1;
			y_vel *= -1;
		}

		
		if (up && !down) y_vel = -SPEED;
		if (down && !up) y_vel = SPEED;
		if (left && !right) x_vel = -SPEED;
		if (right && !left) x_vel = SPEED;


		x_pos += x_vel / 60;
		y_pos += y_vel / 60;

		//collision detection with bounds
		if (x_pos <= 0) {
			x_pos = 0;
		}
		if (y_pos <= 0) {
			y_pos = 0;
		}
		if (x_pos >= WINDOW_WIDTH - dest.w) {
			//printf("X_pos: %f, barier: %i\n", x_pos, WINDOW_WIDTH - dest.w);
			x_pos = WINDOW_WIDTH - dest.w;
		}
		if (y_pos >= WINDOW_HEIGHT - dest.h) {
			//printf("Y_pos: %f, barier: %i\n", y_pos, WINDOW_HEIGHT - dest.h);
			y_pos = WINDOW_HEIGHT - dest.h;
		}

		//set the y positon in the struct
		dest.x = (int)x_pos;
		dest.y = (int)y_pos;
		
		//clear the window
		SDL_RenderClear(rend);

		// draw the image to the window
		SDL_RenderCopy(rend, tex, NULL, &dest);
		SDL_RenderPresent(rend);


		//wait 1/60th of a second
		SDL_Delay(1000 / 60);
	}


	//Clean up resources before exiting
	SDL_DestroyTexture(tex);
	SDL_DestroyRenderer(rend);
	SDL_DestroyWindow(win);
	SDL_Quit();
	return 0;
}