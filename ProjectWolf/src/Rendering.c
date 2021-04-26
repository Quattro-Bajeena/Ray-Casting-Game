#include "stdafx.h"
#include "Rendering.h"


void ClearBuffer(unsigned char* pixelBuffer, Vector2i windowSize) {
	memset(pixelBuffer, 0, windowSize.x * windowSize.y * 4);
}

void DrawPixel(int x, int y , unsigned char r, unsigned char g, unsigned char b, unsigned char* pixelBuffer, Vector2i windowSize) {
	pixelBuffer[((y * windowSize.x) + x) * 4] = r;
	pixelBuffer[((y * windowSize.x) + x) * 4 + 1] = g;
	pixelBuffer[((y * windowSize.x) + x) * 4 + 2] = b;
	pixelBuffer[((y * windowSize.x) + x) * 4 + 3] = 255;
}


void DrawPlayer(const Player player, SDL_Renderer* renderer)
{
	SDL_Rect playerRect = { player.pos.x - 4,player.pos.y - 4, 8, 8 };
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
	SDL_RenderFillRect(renderer, &playerRect);


	Vector2f deltaRot = {
		player.dir.x * cos(M_PI / 2) - player.dir.y * sin(M_PI / 2),
		player.dir.x * sin(M_PI / 2) + player.dir.y * cos(M_PI / 2)
	};

	float thickness = 0.3;
	int length = 20;
	SDL_Point points[] = {
		{ player.pos.x + deltaRot.x * thickness, player.pos.y + deltaRot.y * thickness },
		{ player.pos.x - deltaRot.x * thickness, player.pos.y - deltaRot.y * thickness },
		{ (player.pos.x + player.dir.x * length) - deltaRot.x * thickness, (player.pos.y + player.dir.y * length) - deltaRot.y * thickness },
		{ (player.pos.x + player.dir.x * length) + deltaRot.x * thickness, (player.pos.y + player.dir.y * length) + deltaRot.y * thickness},
		{ player.pos.x + deltaRot.x * thickness, player.pos.y + deltaRot.y * thickness }
	};

	SDL_RenderDrawLines(renderer, points, 5);
}

void DrawMap(Vector2i mapSize, int squareSize, int* map, SDL_Renderer* renderer)
{
	int xo, yo;
	for (int y = 0; y < mapSize.y; y++) {
		for (int x = 0; x < mapSize.x; x++) {

			int wallValue = map[y * mapSize.x + x];

			if (wallValue < 0) {
				SDL_SetRenderDrawColor(renderer, 0, 100, 0, 255);
			}
			else if (wallValue >= 1) {
				SDL_SetRenderDrawColor(renderer, 220, 220, wallValue * 50, 255);
			}
			else {
				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			}
				
			xo = x * squareSize; yo = y * squareSize;

			SDL_Rect mapTileRect = { xo + 1, yo + 1, squareSize - 1, squareSize - 1 };
			SDL_RenderFillRect(renderer, &mapTileRect);

		}
	}
}




RayInfo CalculateRayHit(RayInfo ray, const Player player, int tileSize, Vector2i mapSize, const int* mapWall) {


	ray.distance = FLT_MAX;

	//-----Check Horizontal lines-----
	/*
	This ray stops at the nearest horizontal line that is a wall
	*/
	int dof = 0;
	float distanceHorizontal = FLT_MAX;
	Vector2f horizontalRay = { 0,0 };
	
	float aTan = -1 / tan(ray.angle);
	
	Vector2f offset;

	/*
				up
				270
			   3PI/2
	left 180		0|360 right
		 PI			0|2PI
				90
			   PI/2
				down
	*/

	//looking up
	if (ray.angle > M_PI) {

		// ray.y is truncated version of player.y, 
		//so it starts on the top edge of tile player is standing on
		ray.hit.y = ((int)player.pos.y / tileSize) * tileSize - 0.0001;
		ray.hit.x = (player.pos.y - ray.hit.y) * aTan + player.pos.x;
		offset.y = -tileSize;
		offset.x = -offset.y * aTan;
	}
	//looking down
	if (ray.angle < M_PI) {

		// ray.y is truncated version of player.y + size of tile
		//so it starts on the bottom  edge of tile player is standing on
		ray.hit.y = ((int)player.pos.y / tileSize) * tileSize + tileSize;
		ray.hit.x = (player.pos.y - ray.hit.y) * aTan + player.pos.x;
		offset.y = tileSize;
		offset.x = -offset.y * aTan;
	}
	//looking stright left or right
	if (ray.angle == 0 || ray.angle == M_PI) {
		ray.hit.x = player.pos.x;
		ray.hit.y = player.pos.y;
		dof = 8;
	}

	//checking horizontal lines
	int mapIndex;
	int wallTextureX = 0, wallTextureY = 0;
	while (dof < 8) {


		mapIndex = MapIndexWorld(ray.hit, mapSize, tileSize);

		//Wall hit
		if (mapIndex >= 0 && mapIndex < mapSize.x * mapSize.y && mapWall[mapIndex] >= 1) {

			dof = 8;
			horizontalRay = ray.hit;
			distanceHorizontal = Distance(player.pos, horizontalRay);
			wallTextureX = mapWall[mapIndex] - 1;
		}
		// Just check the next horizontal line
		else {
			ray.hit.x += offset.x;
			ray.hit.y += offset.y;
			dof += 1;
		}
	}


	//---Check VERTICAL lines----
	/*
	This ray stops at the nearest vertical line that is a wall
	*/
	//Almost the same as horiznotal, but Xs and Ys are flipped
	// And we're checking for left and right, not up and down, so difernt angles in ifs
	dof = 0;
	float negativeTan = -tan(ray.angle);
	float distanceVertical = FLT_MAX;
	Vector2f verticalRay = { 0,0 };

	//looking left
	if (ray.angle > M_PI / 2 && ray.angle < 3 * M_PI / 2) {


		ray.hit.x = ((int)player.pos.x / tileSize) * tileSize - 0.0001;
		ray.hit.y = (player.pos.x - ray.hit.x) * negativeTan + player.pos.y;
		offset.x = -tileSize;
		offset.y = -offset.x * negativeTan;
	}
	//looking right
	if (ray.angle < M_PI / 2 || ray.angle>3 * M_PI / 2) {

		ray.hit.x = ((int)player.pos.x / tileSize) * tileSize + tileSize;
		ray.hit.y = (player.pos.x - ray.hit.x) * negativeTan + player.pos.y;
		offset.x = tileSize;
		offset.y = -offset.x * negativeTan;
	}
	//looking stright up or down
	if (ray.angle == 0 || ray.angle == M_PI) {
		ray.hit.y = player.pos.y;
		ray.hit.x = player.pos.x;
		dof = 8;
	}

	//checking horizontal lines
	while (dof < 8) {


		mapIndex = MapIndexWorld(ray.hit, mapSize, tileSize);

		//Wall hit
		if (mapIndex >= 0 && mapIndex < mapSize.x * mapSize.y && mapWall[mapIndex] >= 1) {
			dof = 8;
			verticalRay = ray.hit;
			distanceVertical = Distance(player.pos, verticalRay);
			wallTextureY = mapWall[mapIndex] - 1;

		}
		// Just check the next horizontal line
		else {
			ray.hit.x += offset.x;
			ray.hit.y += offset.y;
			dof += 1;
		}
	}


	// Ray Hit horizontal Wall
	if (distanceVertical < distanceHorizontal) {
		ray.hit = verticalRay;
		ray.distance = distanceVertical;
		ray.wallDirection = vertical;
		ray.wallTexture = wallTextureY;
	}
	//Ray hit vertical wall
	else {
		ray.hit = horizontalRay;
		ray.distance = distanceHorizontal;
		ray.wallDirection = horizontal;
		ray.wallTexture = wallTextureX;
	}
	
	return ray;
}





TextureDrawInfo CalculateWallTexture(RayInfo ray, int *lineHeight, int textureSize, Vector2i renderSize) {
	TextureDrawInfo texInfo;

	// we need a distance not of ray to the wall,
	// but it projected onto vector in line with player look angle
	texInfo.vertStep = textureSize / (float)(*lineHeight); //ratio of texture pixels to wall pixel height
	texInfo.vertOffset = 0;
	texInfo.shading = 1;

	if (*lineHeight > renderSize.y) {
		//lineHeight is still unclammped, can be bigger than screen size
		texInfo.vertOffset = (*lineHeight - renderSize.y) / 2.0;
		(*lineHeight) = renderSize.y; //clamping line height to screen height

	}

	//if not the whole wall is visible, it will start drawing it not from top
	texInfo.cords.y = (texInfo.vertOffset * texInfo.vertStep) + (ray.wallTexture * textureSize);

	// Orienting textures on walls
	// cords = (texture_size-1) - cords  == flipping it
	if (ray.wallDirection == horizontal) {
		texInfo.shading = 0.7f;
		texInfo.cords.x = (int)(ray.hit.x / 2) % textureSize;
		if (ray.angle < 180 * DEG) {
			texInfo.cords.x = (textureSize - 1) - texInfo.cords.x;
		}
	}
	else if (ray.wallDirection == vertical) {
		texInfo.shading = 1;
		texInfo.cords.x = (int)(ray.hit.y / 2) % textureSize;
		if (ray.angle > 90 * DEG && ray.angle < 270 * DEG) {
			texInfo.cords.x = (textureSize - 1) - texInfo.cords.x;
		}
	}

	return texInfo;
}

void DrawRays(const Player player, float FOV_deg,
	int* mapWall, int* mapFloor, int* mapCeiling, 
	Vector2i mapSize, int tileSize, int textureSize, int* Textures,
	SDL_Renderer* rendererMap, int* pixelBuffer, Vector2i renderSize)
{
	float FOV = FOV_deg * DEG;

	RayInfo ray;
	ray.angle = player.angle - (FOV / 2);
	ray.angle = AngleRollOver(ray.angle);

	float rayAngleIncrement = FOV / renderSize.x;


	for (int row = 0; row < renderSize.x; row++) {
		ray = CalculateRayHit(ray, player, tileSize, mapSize, mapWall);

		SDL_SetRenderDrawColor(rendererMap, 180, 180, 180, 80);
		SDL_RenderDrawLine(rendererMap, player.pos.x, player.pos.y, ray.hit.x, ray.hit.y);


		//----DRAW 3D WALLS ----
		float cosAngle = AngleRollOver(player.angle - ray.angle);
		ray.distance *= cos(cosAngle); // fix fisheye

		// Walls with distance of one tile will have height of the screen
		int lineHeight = ((tileSize * renderSize.y) / ray.distance); // line height can be bigger than the screen height if wall is so close it isn't seen
		TextureDrawInfo texInfo = CalculateWallTexture(ray, &lineHeight, textureSize, renderSize);
		int lineOffset = renderSize.y / 2 - lineHeight / 2; //offset from top of the screen, 0 if it takes up entire space

		// Actual Drawing
		for (int y = 0; y < lineHeight; y++) {
			float color = Textures[(int)texInfo.cords.y * textureSize + (int)texInfo.cords.x] * texInfo.shading * 255;
			texInfo.cords.y += texInfo.vertStep;

			DrawPixel(row, lineOffset + y, color, color, color, pixelBuffer, renderSize);
		}


		// --- DRAW FLOORS ---

		/*for (int y = lineOffset + lineHeight; y < WINDOW_HEIGHT; y++) {
			float floorDeltaY = y - (WINDOW_HEIGHT / 2);
			float floorRayAngle = cos(AngleRollOver(player.angle - rayAngle));
			Vector2f texturePos = {
				player.pos.x / 2 + cos(rayAngle) * 158 * TEXTURE_SIZE / floorDeltaY / floorRayAngle,
				player.pos.y / 2 + cos(rayAngle) * 158 * TEXTURE_SIZE / floorDeltaY / floorRayAngle
			};
			int mapPos = mapFloor[(int)(texturePos.y / TEXTURE_SIZE) * mapSize.x + (int)(texturePos.x / TEXTURE_SIZE)] * TEXTURE_SIZE * TEXTURE_SIZE;
			float color = All_Textures[((int)(texturePos.y) & (TEXTURE_SIZE - 1)) * TEXTURE_SIZE + ((int)(texturePos.x) & (TEXTURE_SIZE - 1)) + mapPos] * 0.7 * 255;

			SDL_SetRenderDrawColor(renderer, color, color, color, 255);
			SDL_Rect tempRect = { row + VIEW_OFFSET ,  y, rayDencity,1 };
			SDL_RenderDrawRect(renderer, &tempRect);

		}*/

		ray.angle += rayAngleIncrement;
		ray.angle = AngleRollOver(ray.angle);
	}

;
}