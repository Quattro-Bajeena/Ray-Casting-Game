#define _CRT_SECURE_NO_WARNINGS


#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0') 


#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>


void BuildTable(const int rows, const int columns, unsigned int**& table, int*& sizes) {

	table = new unsigned int*[rows];
	sizes = new int[rows];
	//(*table) = (int**)malloc(rows * sizeof(unsigned int*));
	//(*sizes) = (int*)malloc(rows * sizeof(int));

	for (int i = 0; i < rows; i++) {
		//(*table)[i] = malloc(columns * sizeof(unsigned int));
		table[i] = new unsigned int[columns];
		//(*sizes)[i] = 0;
		sizes[i] = 0;
	}

	//printf("%p\n%p\n", (void *)(*table), (void*)(*sizes));

}

void Insert(unsigned int input, unsigned int** table, int* sizes) {
	unsigned int lastBits = input & 0b111;
	table[lastBits][sizes[lastBits]] = input;
	sizes[lastBits]++;
}

void ReduceTable(int rows, unsigned int** table, int* sizes) {
	for (int i = 0; i < rows; i++) {
		// propably will work, but UB when allocating memory with new
		table[i] = (unsigned int*)realloc(table[i], sizes[i] * sizeof(unsigned int));
		printf("Row %i: size: %i\n", i, sizes[i]);
	}
}

void PrintTable2(int rows, unsigned int** table, int* sizes) {
	for (int i = 0; i < rows; i++) {

		printf("%u: ", i);
		for (int j = 0; j < sizes[i]; j++) {

			printf("%u|" BYTE_TO_BINARY_PATTERN" ", table[i][j], BYTE_TO_BINARY(table[i][j]));
		}
		puts("");
	}
}

void l4ex6() {

	int rows = 8;
	int columns = 10;

	unsigned int** table = NULL;
	int* sizes = NULL;


	BuildTable(rows, columns, table, sizes);

	unsigned int input = 0;

	scanf("%u", &input);
	while (input != 0) {
		Insert(input, table, sizes);
		scanf("%u", &input);
	}

	ReduceTable(rows, table, sizes);

	printf("Table before reducing: %i by %i\n", rows, columns);

	PrintTable2(rows, table, sizes);

	for(int i = 0; i < rows; i++)
		delete[] table[i];
	delete[] table;
	delete[] sizes;
}

int l4e7main() {
	l4ex6();


	return 0;
}