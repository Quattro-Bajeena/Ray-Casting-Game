#pragma once
#include "sdl_util.h"



int randInt(int min, int max);
int randomColor();
float Distance(Vector2f a, Vector2f b);

typedef enum { vertical, horizontal } WallDirection;

Vector2i WorldToGridPos(Vector2f worldPos, int tileSize);
int MapIndexMap(Vector2i posMapCords, Vector2i mapSize);
int MapIndexWorld(Vector2f posWorldCords, Vector2i mapSize, float tileSize);
float AngleRollOver(float angle);
