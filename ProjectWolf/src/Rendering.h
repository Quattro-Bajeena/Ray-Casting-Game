#pragma once
#include "sdl_util.h"
#include "utils.h"




void DrawPlayer(Player* player, SDL_Renderer* renderer);
void DrawMap(Vector2i mapSize, int squareSize, int* map, SDL_Renderer* renderer);
void DrawRays(Player* player, int* mapWall, int* mapFloor, int* mapCeiling, Vector2i mapSize, int tileSize, int* Textures, SDL_Renderer* renderer);