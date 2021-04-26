#pragma once
#include "sdl_util.h"
#include "utils.h"
#include "PlayerLogic.h"


typedef struct {
	Vector2f cords;
	float vertOffset;
	float vertStep;
	float shading;
} TextureDrawInfo;

typedef struct {
	float angle, distance;
	Vector2f hit;
	WallDirection wallDirection;
	int wallTexture;
} RayInfo;

void ClearBuffer(unsigned char* pixelBuffer, Vector2i windowSize);
void DrawPixel(int width, int height, unsigned char r, unsigned char g, unsigned char b, unsigned char* pixelBuffer, Vector2i windowSize);
void DrawPlayer(Player* player, SDL_Renderer* renderer);
void DrawMap(Vector2i mapSize, int squareSize, int* map, SDL_Renderer* renderer);

RayInfo CalculateRayHit(RayInfo ray, const Player* player, int tileSize, Vector2i mapSize, const int* mapWall);
TextureDrawInfo CalculateWallTexture(RayInfo ray, int lineHeight, int textureSize, Vector2i renderSize);
void DrawRays(Player* player, float FOV_deg,
	int* mapWall, int* mapFloor, int* mapCeiling,
	Vector2i mapSize, int tileSize, int textureSize, int* Textures,
	SDL_Renderer* rendererMap, int* pixelBuffer, Vector2i renderSize, Vector2i screenMap);