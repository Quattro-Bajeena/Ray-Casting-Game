#include "stdafx.h"
#include "PlayerLogic.h"

void PlayerMovement(Player* player, InputDir inputDir,
	int* map, int tileSize, Vector2i mapSize,
	float deltaTime) {


	if (inputDir.left) {
		player->angle -= 0.1;
		player->dir.x = cos(player->angle) * player->speed * deltaTime;
		player->dir.y = sin(player->angle) * player->speed * deltaTime;
	}
	if (inputDir.right) {
		player->angle += 0.1;
		player->dir.x = cos(player->angle) * player->speed * deltaTime;
		player->dir.y = sin(player->angle) * player->speed * deltaTime;
	}

	player->angle = AngleRollOver(player->angle);

	//printf("%f\n", *playerAngle);

	Vector2i offset = { 0,0 };
	offset.x = player->dir.x < 0 ? -20 : 20;
	offset.y = player->dir.y < 0 ? -20 : 20;
	Vector2i playerGridAddOffset = {
		(player->pos.x + offset.x) / tileSize,
		(player->pos.y + offset.y) / tileSize
	};
	Vector2i playerGridMinOffset = {
		(player->pos.x - offset.x) / tileSize,
		(player->pos.y - offset.y) / tileSize
	};
	Vector2i playerGrid = WorldToGridPos(player->pos, tileSize);


	if (inputDir.up) {
		// Only moves if the position of player + offset is not in a wall
		if (map[playerGrid.y * mapSize.x + playerGridAddOffset.x] == 0) { player->pos.x += player->dir.x; }
		if (map[playerGridAddOffset.y * mapSize.x + playerGrid.x] == 0) { player->pos.y += player->dir.y; }

	}
	if (inputDir.down) {
		if (map[playerGrid.y * mapSize.x + playerGridMinOffset.x] == 0) { player->pos.x -= player->dir.x; }
		if (map[playerGridMinOffset.y * mapSize.x + playerGrid.x] == 0) { player->pos.y -= player->dir.y; }
	}
}

void PlayerInteraction(Player* player, int* mapWalls, int tileSize, Vector2i mapSize) {

	int interactDistance = 25;
	Vector2i interactOffset = { 0,0 };
	interactOffset.x = player->dir.x < 0 ? -interactDistance : interactDistance;
	interactOffset.y = player->dir.y < 0 ? -interactDistance : interactDistance;
	Vector2i interactionGridAddOffset = {
		((player->pos).x + interactOffset.x) / tileSize,
		((player->pos).y + interactOffset.y) / tileSize
	};
	Vector2i playerGrid = WorldToGridPos(player->pos, tileSize);

	if (player->actions.interact == true) {
		int interactionMapIndex = interactionGridAddOffset.y * mapSize.x + interactionGridAddOffset.x;
		if (mapWalls[interactionMapIndex] == 4) {
			mapWalls[interactionMapIndex] = 0;
		}
	}

}