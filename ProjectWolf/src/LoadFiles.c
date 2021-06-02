#include "stdafx.h"
#include "LoadFiles.h"

int* LoadMap(char filename[], Vector2i* size)
{
	FILE* fp = fopen(filename, "r");
	if (fp == NULL) {
		printf("Error opening the file\n");
		return NULL;
	}

	
	int c;
	fscanf(fp, "%i %i", &(size->x), &(size->y));

	int* map = malloc(size->x * size->y * sizeof(int));
	int index = 0;

	while ((c = fgetc(fp)) != EOF) {
		if (c != '\n') {
			map[index++] = c - '0';
		}	
	}

	fclose(fp);
	return map;
}
