#include "stdafx.h"
#include "utils.h"

int randInt(int min, int max) {
	static bool initialized = false;
	if (!initialized) {
		srand(time(NULL));
		initialized = true;
	}
		

	return rand() % max + min;
}

int randomColor() {
	return randInt(0, 255);
}

float Distance(Vector2f a, Vector2f b)
{
	return sqrt((b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y));
}

Vector2i WorldToGridPos(Vector2f worldPos, int tileSize) {
	Vector2i GridPos = { worldPos.x / tileSize, worldPos.y / tileSize };
	return GridPos;
}

int MapIndexMap(Vector2i posMapCords, Vector2i mapSize) {
	return posMapCords.y * mapSize.x + posMapCords.x;
}

int MapIndexWorld(Vector2f posWorldCords, Vector2i mapSize, float tileSize) {
	int mapPosX = (int)(posWorldCords.x) / tileSize;
	int mapPosY = (int)(posWorldCords.y) / tileSize;

	return mapPosY * mapSize.x + mapPosX;
}

float AngleRollOver(float angle) {
	if (angle < 0) { return angle += 2 * M_PI; }
	if (angle > 2 * M_PI) { return angle -= 2 * M_PI; }
	else return angle;
}
