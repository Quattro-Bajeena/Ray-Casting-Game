#include "stdafx.h"
#include "PlayerLogic.h"


Player InitPlayer() {
	Player player;
	player.pos.x = 200;	player.pos.y = 200;
	player.angle = 0;
	player.speed = 150;
	player.turningSpeed = 0.05;
	player.dir.x = cos(player.angle); 	player.dir.y = sin(player.angle);
	return player;
}

void PlayerMovement(Player* player, InputDir inputDir,
	int* map, int tileSize, Vector2i mapSize,
	float deltaTime) {

	
	if (inputDir.left) {
		player->angle -= player->turningSpeed;
		player->dir.x = cos(player->angle) ;
		player->dir.y = sin(player->angle) ;
	}
	if (inputDir.right) {
		player->angle += player->turningSpeed;
		player->dir.x = cos(player->angle) ;
		player->dir.y = sin(player->angle) ;
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
		if (map[playerGrid.y * mapSize.x + playerGridAddOffset.x] <= 0) { player->pos.x += player->dir.x * player->speed * deltaTime; }
		if (map[playerGridAddOffset.y * mapSize.x + playerGrid.x] <= 0) { player->pos.y += player->dir.y * player->speed * deltaTime; }

	}
	if (inputDir.down) {
		if (map[playerGrid.y * mapSize.x + playerGridMinOffset.x] <= 0) { player->pos.x -= player->dir.x * player->speed * deltaTime; }
		if (map[playerGridMinOffset.y * mapSize.x + playerGrid.x] <= 0) { player->pos.y -= player->dir.y * player->speed * deltaTime; }
	}
}

void PlayerInteraction(Player* player, int* mapWalls, int tileSize, Vector2i mapSize) {

	//Sometinhg fucky is going on
	int interactDistance = tileSize/2;

	Vector2i interactionGridAddOffset = {
		((player->pos).x + player->dir.x * interactDistance) / tileSize,
		((player->pos).y + player->dir.y * interactDistance) / tileSize
	};
	


	if (player->actions.interact == true) {

		int interactionMapIndex = interactionGridAddOffset.y * mapSize.x + interactionGridAddOffset.x;

		
		if (mapWalls[interactionMapIndex] == 4) {
			mapWalls[interactionMapIndex] = 0;
		}
		else if (mapWalls[interactionMapIndex] == 0) {
			mapWalls[interactionMapIndex] = -1;
		}
	}

}