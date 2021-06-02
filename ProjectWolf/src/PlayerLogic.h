#pragma once
#include "utils.h"

typedef struct Player{
	Vector2f pos;
	Vector2f dir;
	float speed;
	float angle;
	float turningSpeed;
	Actions actions;
} Player;

Player InitPlayer();
void PlayerMovement(Player* player, InputDir inputDir,
	int* map, int tileSize, Vector2i mapSize,
	float deltaTime);

void PlayerInteraction(Player* player, int* mapWalls, int tileSize, Vector2i mapSize);