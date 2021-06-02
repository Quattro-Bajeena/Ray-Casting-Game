#pragma once
#include "sdl_util.h"
#include "utils.h"
#include "PlayerLogic.h"


typedef struct TextureDrawInfo {
	Vector2f cords;
	float vertStep;
	float shading;
} TextureDrawInfo;

typedef struct RayInfo {
	float angle, distance;
	Vector2f hit;
	WallDirection wallDirection;
	int wallTexture;
} RayInfo;

//SDL_Color GetPixelColor(int x, int y, SDL_Surface* texture);
void ClearBuffer(SDL_Color* pixelBuffer,Vector2i windowSize);
//void DrawPixel(int x, int y, SDL_Color color, SDL_Color* pixelBuffer, int windowWidth);
void DrawPlayer(const Player player, SDL_Renderer* renderer);
void DrawMap(Vector2i mapSize, int squareSize, int* map, SDL_Texture** textures, SDL_Renderer* renderer);

//RayInfo CalculateRayHit(RayInfo ray, const Player player, int tileSize, Vector2i mapSize, const int* mapWall);
//TextureDrawInfo CalculateWallTexture(RayInfo ray, int* lineHeight, int textureSize, Vector2i renderSize);
void DrawRays(const Player player, float FOV_deg,
	int* mapWall,
	Vector2i mapSize, int tileSize, int textureSize, SDL_Surface* Textures[],
	SDL_Renderer* rendererMap, SDL_Color* pixelBuffer, Vector2i renderSize);