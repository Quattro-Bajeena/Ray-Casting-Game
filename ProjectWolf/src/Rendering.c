#include "stdafx.h"
#include "Rendering.h"


void DrawPlayer(Player* player, SDL_Renderer* renderer)
{
	SDL_Rect playerRect = { player->pos.x - 4,player->pos.y - 4, 8, 8 };
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
	SDL_RenderFillRect(renderer, &playerRect);

	//SDL_RenderSetScale(renderer, 2, 2);
	Vector2f deltaRot = {
		player->dir.x * cos(M_PI / 2) - player->dir.y * sin(M_PI / 2),
		player->dir.x * sin(M_PI / 2) + player->dir.y * cos(M_PI / 2)
	};

	float thickness = 0.3;
	int length = 6;
	SDL_Point points[] = {
		{ player->pos.x + deltaRot.x * thickness, player->pos.y + deltaRot.y * thickness },
		{ player->pos.x - deltaRot.x * thickness, player->pos.y - deltaRot.y * thickness },
		{ (player->pos.x + player->dir.x * length) - deltaRot.x * thickness, (player->pos.y + player->dir.y * length) - deltaRot.y * thickness },
		{ (player->pos.x + player->dir.x * length) + deltaRot.x * thickness, (player->pos.y + player->dir.y * length) + deltaRot.y * thickness},
		{ player->pos.x + deltaRot.x * thickness, player->pos.y + deltaRot.y * thickness }
	};

	SDL_RenderDrawLines(renderer, points, 5);
}

void DrawMap(Vector2i mapSize, int squareSize, int* map, SDL_Renderer* renderer)
{
	int xo, yo;
	for (int y = 0; y < mapSize.y; y++) {
		for (int x = 0; x < mapSize.x; x++) {



			if (map[y * mapSize.x + x] >= 1)
				SDL_SetRenderDrawColor(renderer, 255, 255, 100, 255);
			else
				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

			xo = x * squareSize; yo = y * squareSize;


			SDL_Rect mapTileRect = { xo + 1, yo + 1, squareSize - 1, squareSize - 1 };
			SDL_RenderFillRect(renderer, &mapTileRect);


		}
	}
}


void DrawRays(Player* player, int* mapWall, int* mapFloor, int* mapCeiling, Vector2i mapSize, int tileSize, int* Textures,  SDL_Renderer* renderer)
{
	float FOV = 60 * DEG;
	int renderWidth = 480;
	Vector2i renderSize = { 480, 512 };
	int rayDencity = 10;

	int mapIndex, dof;
	Vector2i mapPos;
	float rayAngle, distance = FLT_MAX;
	Vector2f ray, offset;

	rayAngle = player->angle - (FOV / 2);
	rayAngle = AngleRollOver(rayAngle);

	float rayAngleIncrement = FOV / renderSize.x;

	//SDL_Point* pixelBuffer = (SDL_Point*)malloc(WINDOW_WIDTH * WINDOW_HEIGHT * sizeof(SDL_Point));
	//int drawnPixelCount = 0;
	for (int row = 0; row < renderSize.x; row += rayDencity) {

		//-----Check Horizontal lines-----
		/*
		This ray stops at the nearest horizontal line that is a wall
		*/
		dof = 0;
		float distanceHorizontal = FLT_MAX;
		Vector2f horizontalRay;
		float aTan = -1 / tan(rayAngle);
		Vector2i wallTextureIndex = { 0,0 };

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
		if (rayAngle > M_PI) {

			// ray.y is truncated version of player.y, 
			//so it starts on the top edge of tile player is standing on
			ray.y = ((int)player->pos.y / tileSize) * tileSize - 0.0001;
			ray.x = (player->pos.y - ray.y) * aTan + player->pos.x;
			offset.y = -tileSize;
			offset.x = -offset.y * aTan;
		}
		//looking down
		if (rayAngle < M_PI) {

			// ray.y is truncated version of player.y + size of tile
			//so it starts on the bottom  edge of tile player is standing on
			ray.y = ((int)player->pos.y / tileSize) * tileSize + tileSize;
			ray.x = (player->pos.y - ray.y) * aTan + player->pos.x;
			offset.y = tileSize;
			offset.x = -offset.y * aTan;
		}
		//looking stright left or right
		if (rayAngle == 0 || rayAngle == M_PI) {
			ray.x = player->pos.x;
			ray.y = player->pos.y;
			dof = 8;
		}

		//checking horizontal lines
		while (dof < 8) {


			mapIndex = MapIndexWorld(ray, mapSize, tileSize);

			//Wall hit
			if (mapIndex >= 0 && mapIndex < mapSize.x * mapSize.y && mapWall[mapIndex] >= 1) {

				dof = 8;
				horizontalRay = ray;
				distanceHorizontal = Distance(player->pos, horizontalRay);
				wallTextureIndex.x = mapWall[mapIndex] - 1;
			}
			// Just check the next horizontal line
			else {
				ray.x += offset.x;
				ray.y += offset.y;
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
		float negativeTan = -tan(rayAngle);
		float distanceVertical = FLT_MAX;
		Vector2f verticalRay;

		//looking left
		if (rayAngle > M_PI / 2 && rayAngle < 3 * M_PI / 2) {


			ray.x = ((int)player->pos.x / tileSize) * tileSize - 0.0001;
			ray.y = (player->pos.x - ray.x) * negativeTan + player->pos.y;
			offset.x = -tileSize;
			offset.y = -offset.x * negativeTan;
		}
		//looking right
		if (rayAngle < M_PI / 2 || rayAngle>3 * M_PI / 2) {

			ray.x = ((int)player->pos.x / tileSize) * tileSize + tileSize;
			ray.y = (player->pos.x - ray.x) * negativeTan + player->pos.y;
			offset.x = tileSize;
			offset.y = -offset.x * negativeTan;
		}
		//looking stright up or down
		if (rayAngle == 0 || rayAngle == M_PI) {
			ray.y = player->pos.y;
			ray.x = player->pos.x;
			dof = 8;
		}

		//checking horizontal lines
		while (dof < 8) {


			mapIndex = MapIndexWorld(ray, mapSize, tileSize);

			//Wall hit
			if (mapIndex >= 0 && mapIndex < mapSize.x * mapSize.y && mapWall[mapIndex] >= 1) {
				dof = 8;
				verticalRay = ray;
				distanceVertical = Distance(player->pos, verticalRay);
				wallTextureIndex.y = mapWall[mapIndex] - 1;

			}
			// Just check the next horizontal line
			else {
				ray.x += offset.x;
				ray.y += offset.y;
				dof += 1;
			}
		}


		// The shortest of Horizontal Rays is drawn
		// This one hit the wall closest
		float shading = 1;
		bool horizontalWall = true;
		WallDirection wallDirection = horizontal;
		if (distanceVertical < distanceHorizontal) {
			ray = verticalRay;
			distance = distanceVertical;
			shading = 0.5;
			wallDirection = vertical;
			wallTextureIndex.x = wallTextureIndex.y;
			SDL_SetRenderDrawColor(renderer, 120, 120, 120, 255);
		}
		if (distanceVertical > distanceHorizontal) {
			ray = horizontalRay;
			distance = distanceHorizontal;
			shading = 1;
			wallDirection = horizontal;
			wallTextureIndex.y = wallTextureIndex.x;
			SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
		}
		SDL_RenderDrawLine(renderer, player->pos.x, player->pos.y, ray.x, ray.y);


		//----DRAW 3D WALLS ----
		// we need a distance not of ray to the wall,
		// but it projected onto vector in line with player look angle
		float cosAngle = AngleRollOver(player->angle - rayAngle);
		distance *= cos(cosAngle); // fix fisheye

		// Walls with distance of one tile will have height of the screen
		int lineHeight = (tileSize * renderSize.y) / distance;
		Vector2f textureStep = { 0 , TEXTURE_SIZE / (float)lineHeight }; //ratio of texture pixels to wall pixel height
		Vector2f textureOffset = { 0,0 };

		if (lineHeight > renderSize.y) {
			textureOffset.y = (lineHeight - renderSize.y) / 2.0;
			lineHeight = renderSize.y;

		}
		float lineOffset = renderSize.y / 2 - lineHeight / 2;


		// yoo adding points to array and only then drawing them is great optimisation
		//SDL_Point points[WINDOW_HEIGHT];

		//if not the whole wall is visible, it will start drawing it not from top
		Vector2f textureCords;
		textureCords.y = (textureOffset.y * textureStep.y) + wallTextureIndex.x * 32;

		// Orienting textures on walls
		// cords = (texture_size-1) - cords
		// == flipping it
		//printf("%f\n", rayAngle);
		if (wallDirection == horizontal) {
			textureCords.x = (int)(ray.x / 2) % TEXTURE_SIZE;
			if (rayAngle < 180 * DEG) {
				textureCords.x = (TEXTURE_SIZE - 1) - textureCords.x;
			}
		}
		else if (wallDirection == vertical) {
			textureCords.x = (int)(ray.y / 2) % TEXTURE_SIZE;
			if (rayAngle > 90 * DEG && rayAngle < 270 * DEG) {
				textureCords.x = (TEXTURE_SIZE - 1) - textureCords.x;
			}
		}

		for (int y = 0; y < lineHeight; y++) {
			//SDL_Point tempPoint= { row * density + VIEW_OFFSET , lineOffset + y };
			//points[y] = tempPoint;
			float color = Textures[(int)textureCords.y * TEXTURE_SIZE + (int)textureCords.x] * shading * 255;
			SDL_SetRenderDrawColor(renderer, color, color, color, 255);
			SDL_Rect tempRect = { row + VIEW_OFFSET , lineOffset + y, rayDencity,1 };
			SDL_RenderDrawRect(renderer, &tempRect);
			textureCords.y += textureStep.y;
		}
		//SDL_RenderDrawPoints(renderer, points, lineHeight);

		// --- DRAW FLOORS ---

		/*for (int y = lineOffset + lineHeight; y < WINDOW_HEIGHT; y++) {
			float floorDeltaY = y - (WINDOW_HEIGHT / 2);
			float floorRayAngle = cos(AngleRollOver(player->angle - rayAngle));
			Vector2f texturePos = {
				player->pos.x / 2 + cos(rayAngle) * 158 * TEXTURE_SIZE / floorDeltaY / floorRayAngle,
				player->pos.y / 2 + cos(rayAngle) * 158 * TEXTURE_SIZE / floorDeltaY / floorRayAngle
			};
			int mapPos = mapFloor[(int)(texturePos.y / TEXTURE_SIZE) * mapSize.x + (int)(texturePos.x / TEXTURE_SIZE)] * TEXTURE_SIZE * TEXTURE_SIZE;
			float color = All_Textures[((int)(texturePos.y) & (TEXTURE_SIZE - 1)) * TEXTURE_SIZE + ((int)(texturePos.x) & (TEXTURE_SIZE - 1)) + mapPos] * 0.7 * 255;

			SDL_SetRenderDrawColor(renderer, color, color, color, 255);
			SDL_Rect tempRect = { row + VIEW_OFFSET ,  y, rayDencity,1 };
			SDL_RenderDrawRect(renderer, &tempRect);

		}*/

		rayAngle += (rayAngleIncrement * rayDencity);
		rayAngle = AngleRollOver(rayAngle);
	}

	//SDL_RenderDrawPoints(renderer, pixelBuffer, drawnPixelCount);
	//free(pixelBuffer);
}