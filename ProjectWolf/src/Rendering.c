#include "stdafx.h"
#include "Rendering.h"

SDL_Color GetPixelColor(int x, int y, SDL_Surface* texture) {
	//Bytes per pixel.  But we know it will be 4 bytes in our case
	int bpp = texture->format->BytesPerPixel;

	/* Here p is the address to the pixel we want to retrieve */
	// Pitch - length of a row of pixels in bytes.
	Uint8* p = (Uint8*)texture->pixels + y * texture->pitch + x * bpp;
	// we have RGBA data, so one pixel has 32bits
	Uint32 pixel = *(Uint32*)p;
	SDL_Color color;
	SDL_GetRGB(pixel, texture->format, &color.r, &color.g, &color.b);
	return color;
}

void ClearBuffer(SDL_Color* pixelBuffer, Vector2i windowSize) {
	memset(pixelBuffer, 0, windowSize.x * windowSize.y * 4);
}



void DrawPixel(int x, int y, SDL_Color color, SDL_Color* pixelBuffer, int windowWidth)
{
	pixelBuffer[y * windowWidth + x] = color;
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

void DrawMap(Vector2i mapSize, int squareSize, int* map, SDL_Texture** textures, SDL_Renderer* renderer)
{
	int xo, yo;
	for (int y = 0; y < mapSize.y; y++) {
		for (int x = 0; x < mapSize.x; x++) {

			int wallValue = map[y * mapSize.x + x];
			xo = x * squareSize; yo = y * squareSize;
			SDL_Rect mapTileRect = { xo + 1, yo + 1, squareSize - 1, squareSize - 1 };

			if (wallValue < 0) {
				SDL_SetRenderDrawColor(renderer, 0, 100, 0, 255);
				SDL_RenderFillRect(renderer, &mapTileRect);
			}
			else if (wallValue >= 1) {
				//SDL_SetRenderDrawColor(renderer, 220, 220, wallValue * 50, 255);
				SDL_RenderCopy(renderer, textures[wallValue - 1], NULL, &mapTileRect);	
			}
			else {
				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
				SDL_RenderFillRect(renderer, &mapTileRect);
			}
				
		}
	}
}




RayInfo CalculateRayHit(float angle, const Player player, int tileSize, Vector2i mapSize, const int* mapWall) {

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
	RayInfo ray;
	ray.angle = angle;
	ray.distance = FLT_MAX;
	int dof = 0;
	Vector2f pointZero = { 0,0 }; Vector2f mapSizeF = { mapSize.x, mapSize.y };
	const int dofLimit = (int)Distance(pointZero, mapSizeF); // Depth of Field limit is the diagonal distance of map, the longest you can possible look



	float aTan = 1 / tan(ray.angle);
	Vector2f offset = { 0,0 };
	// checking ----HORIZONTAL---- lines
	//This ray stops at the nearest horizontal line that is a wall
	//looking up
	if (ray.angle > M_PI) {
		// ray.y is truncated version of player.y, 
		//so it starts on the top edge of tile player is standing on
		ray.hit.y = ((int)player.pos.y / tileSize) * tileSize -0.0001;
		offset.y = -tileSize;
	}
	//looking down
	if (ray.angle < M_PI) {

		// ray.y is truncated version of player.y + size of tile
		//so it starts on the bottom  edge of tile player is standing on
		ray.hit.y = ((int)player.pos.y / tileSize) * tileSize + tileSize;
		offset.y = tileSize;
	}
	// Calculting the same way for both directions
	// but won't be THE same, because of difernt ray.hi.y and offset.y ofc
	ray.hit.x = (ray.hit.y - player.pos.y) * aTan + player.pos.x;
	offset.x = offset.y * aTan;

	//looking stright left or right
	if (ray.angle == 0 || ray.angle == M_PI) {
		ray.hit.x = player.pos.x;
		ray.hit.y = player.pos.y;
		dof = dofLimit;
	}

	//checking horizontal lines
	int mapIndex;
	int wallTextureX = 0; 
	float distanceHorizontal = FLT_MAX;
	Vector2f horizontalRay = { 0,0 };
	while (dof < dofLimit) {


		mapIndex = MapIndexWorld(ray.hit, mapSize, tileSize);

		//Wall hit
		if (mapIndex >= 0 && mapIndex < mapSize.x * mapSize.y && mapWall[mapIndex] >= 1) {

			dof = dofLimit;
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
	float tangent = tan(ray.angle);
	
	//looking left
	if (ray.angle > M_PI / 2 && ray.angle < 3 * M_PI / 2) {
		ray.hit.x = ((int)player.pos.x / tileSize) * tileSize -0.0001;
		offset.x = -tileSize;
		
	}
	//looking right
	if (ray.angle < M_PI / 2 || ray.angle>3 * M_PI / 2) {
		ray.hit.x = ((int)player.pos.x / tileSize) * tileSize + tileSize;
		offset.x = tileSize;
	}
	ray.hit.y = (ray.hit.x - player.pos.x) * tangent + player.pos.y;
	offset.y = offset.x * tangent;

	//looking stright up or down
	if (ray.angle == 0 || ray.angle == M_PI) {
		ray.hit.y = player.pos.y;
		ray.hit.x = player.pos.x;
		dof = dofLimit;
	}

	int wallTextureY = 0;
	float distanceVertical = FLT_MAX;
	Vector2f verticalRay = { 0,0 };
	//checking horizontal lines
	while (dof < dofLimit) {
		mapIndex = MapIndexWorld(ray.hit, mapSize, tileSize);

		//Wall hit
		if (mapIndex >= 0 && mapIndex < mapSize.x * mapSize.y && mapWall[mapIndex] >= 1) {
			dof = dofLimit;
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

	RayInfo finalRay;
	finalRay.angle = ray.angle;
	// Ray Hit horizontal Wall
	if (distanceVertical < distanceHorizontal) {
		finalRay.hit = verticalRay;
		finalRay.distance = distanceVertical;
		finalRay.wallDirection = vertical;
		finalRay.wallTexture = wallTextureY;
	}
	//Ray hit vertical wall
	else {
		finalRay.hit = horizontalRay;
		finalRay.distance = distanceHorizontal;
		finalRay.wallDirection = horizontal;
		finalRay.wallTexture = wallTextureX;
	}
	
	return finalRay;
}





TextureDrawInfo CalculateWallTexture(RayInfo ray, int *lineHeight, int textureSize, int tileSize ,Vector2i renderSize) {
	TextureDrawInfo texInfo;
	texInfo.vertStep = textureSize / (float)(*lineHeight); //ratio of texture pixels to wall pixel height. how much 1 pixel down on screen is in texture space

	float vertOffset = 0;
	//lineHeight is still unclammped, can be bigger than screen size
	if (*lineHeight > renderSize.y) {
		vertOffset = (*lineHeight - renderSize.y) / 2.0; // offset from the top of the screen upwards (beyond it) in screen space pixels
		(*lineHeight) = renderSize.y; //clamping line height to screen height

	}
	//if not the whole wall is visible, it will start drawing it not from top
	texInfo.cords.y = (vertOffset * texInfo.vertStep);

	// Orienting textures on walls
	// cords = (texture_size-1) - cords  == flipping it
	float tileTextureRatio = (float)tileSize / textureSize; 
	if (ray.wallDirection == horizontal) {
		//texInfo.shading = 0.7f;
		// using the fact that they're placed on a grid we can get x texure coordinates by modulo world space hit position
		//texInfo.cords.x = (int)(ray.hit.x / tileTextureRatio) % textureSize; 
		texInfo.cords.x = ((int)ray.hit.x % tileSize) / tileTextureRatio; //eqivalent methods

		// If ray is oriented up, texture is flipped
		if (ray.angle < 180 * DEG) {
			texInfo.cords.x = (textureSize - 1) - texInfo.cords.x;
		}
	}
	else if (ray.wallDirection == vertical) {
		//texInfo.shading = 1;
		//texInfo.cords.x = (int)(ray.hit.y / tileTextureRatio) % textureSize; 
		texInfo.cords.x = ((int)ray.hit.y % tileSize) / tileTextureRatio;

		// If ray is oriented left, texture is flipped
		if (ray.angle > 90 * DEG && ray.angle < 270 * DEG) {
			texInfo.cords.x = (textureSize - 1) - texInfo.cords.x;
		}
	}

	return texInfo;
}

void DrawRays(const Player player, float FOV_deg,
	int* mapWall, 
	Vector2i mapSize, int tileSize, int textureSize, SDL_Surface* Textures[],
	SDL_Renderer* rendererMap, SDL_Color* pixelBuffer, Vector2i renderSize)
{
	float FOV = FOV_deg * DEG;

	// initial angle is player - half FOV. last angle will be player's + half FOV
	float currentAngle = AngleRollOver(player.angle - (FOV / 2));
	float rayAngleIncrement = FOV / renderSize.x;

	// every column of pixels on screen is a single ray
	for (int column = 0; column < renderSize.x; column++) {

		// Main part of the algorithm, calculating the coordinates of the ray hit
		RayInfo ray = CalculateRayHit(currentAngle, player, tileSize, mapSize, mapWall);
		currentAngle = AngleRollOver(currentAngle + rayAngleIncrement);

		SDL_SetRenderDrawColor(rendererMap, 180, 180, 180, 80);
		SDL_RenderDrawLine(rendererMap, player.pos.x, player.pos.y, ray.hit.x, ray.hit.y);


		//----DRAW 3D WALLS ----
		// we need a distance not of ray to the wall,
		// but it projected onto vector in line with player look angle
		// fix fisheye
		float cosAngle = AngleRollOver(player.angle - ray.angle);
		ray.distance *= cos(cosAngle); 

		// Walls with distance of one tile will have height of the screen
		// line height can be bigger than the screen height if wall is so close it isn't seen in full
		int lineHeight = ((tileSize * renderSize.y) / ray.distance); 

		// get the texture id, texture x coordiate, starting y coordinate and y step.
		TextureDrawInfo texInfo = CalculateWallTexture(ray, &lineHeight, textureSize, tileSize, renderSize);
		
		// pixel offset from top of the screen, 0 if it takes up entire space. 
		// if lineHeight was taller than screenm, it was set to height of screen in CalculateWallTexture
		int lineOffset = (renderSize.y - lineHeight) / 2;

		// Actual Drawing
		SDL_Color color = { 0,0,0, 255 };
		for (int y = 0; y < lineHeight; y++) {
			color = GetPixelColor(texInfo.cords.x, texInfo.cords.y, Textures[ray.wallTexture]);
			DrawPixel(column, lineOffset + y, color, pixelBuffer, renderSize.x);

			texInfo.cords.y += texInfo.vertStep;
		}

		
	}

}