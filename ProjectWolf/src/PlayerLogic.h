#pragma once
#include "utils.h"


void PlayerMovement(Player* player, InputDir inputDir,
	int* map, int tileSize, Vector2i mapSize,
	float deltaTime);

void PlayerInteraction(Player* player, int* mapWalls, int tileSize, Vector2i mapSize);