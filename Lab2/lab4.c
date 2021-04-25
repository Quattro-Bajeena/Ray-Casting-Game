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


void InputString(char table[128][2]) {
	char str[128]; //= "qwertyuiopasdfghjklzxcvbnmqwertyuiopasdfghjkzxcvnsdfsdfes";
	scanf("%s", &str);

	for (int i = 0; i < strlen(str); i++) {

		for (int j = 0; j < 128; j++) {
			if (str[i] == table[j][0]) {
				table[j][1] += 1;
			}
		}
				
	}
}

void PrintTable(char table[128][2]) {
	for (int i = 0; i < 128; i++) {
		if (table[i][1] > 0) {
			printf("%c : %i\n", table[i][0], table[i][1]);
		}
	}
}



void SortTable(char table[128][2]) {
	

	bool swapped = false;
	int sorted = 128;
	do {
		swapped = false;
		for (int i = 0; i < sorted - 1; i++) {
			if (table[i][1] < table[i + 1][1]) {
				char tempChar = table[i][0];
				char tempNum = table[i][1];

				table[i][0] = table[i + 1][0];
				table[i][1] = table[i + 1][1];

				table[i + 1][0] = tempChar;
				table[i + 1][1] = tempNum;
				swapped = true;
			}
		}


	} while (swapped == true);

}

void l4ex5() {

	char table[128][2];
	for (int i = 0; i < 128; i++) {
		table[i][0] = (char)i;
		table[i][1] = 0;
	}

	InputString(table);
	SortTable(table);
	PrintTable(table);

}

void BuildTable(int rows, int columns, unsigned int*** table, int** sizes) {

	(*table) = (int**)malloc(rows * sizeof(unsigned int*));
	(*sizes) = (int*)malloc(rows * sizeof(int));

	for (int i = 0; i < rows; i++) {
		(*table)[i] = malloc(columns * sizeof(unsigned int));
		(*sizes)[i] = 0;
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
		table[i] = realloc(table[i], sizes[i] * sizeof(unsigned int));
		printf("Row %i: size: %i\n", i, sizes[i]);
	}
}

void PrintTable2(int rows, unsigned int** table,  int* sizes) {
	for (int i = 0; i < rows; i++) {

		printf("%u: ", i);
		for (int j = 0; j < sizes[i]; j++) {

			printf("%u|"BYTE_TO_BINARY_PATTERN" ", table[i][j], BYTE_TO_BINARY(table[i][j]));
		}
		puts("");
	}
}

void l4ex6() {
	
	int rows = 8;
	int columns = 10;

	unsigned int** table = NULL;
	int* sizes = NULL;


	BuildTable(rows, columns, &table, &sizes);

	unsigned int input = 0;

	scanf("%u", &input);
	while (input != 0) {
		Insert(input, table, sizes);
		scanf("%u", &input);
	}

	ReduceTable(rows, table, sizes);
	
	printf("Table before reducing: %i by %i\n", rows, columns);

	PrintTable2(rows, table, sizes);

	for (int i = 0; i < rows; i++)
		free(table[i]);
	free( table);
	free( sizes);

}

void l4ex8() {
	int strSize = 64;

	char* str1 = malloc(strSize * sizeof(char));
	char* str2 = malloc(strSize * sizeof(char));

	puts("Input first word");
	scanf("%s", str1);
	puts("Input second word");
	scanf("%s", str2);

	//strcpy(str1, "Konstantynopol");
	//strcpy(str2, "statuetka");


	int start = strlen(str1), end = strlen(str2);

	for (int i = 0; i < strlen(str1); i++) {

		if (str1[i] == str2[0]) {
			start = i;
			break;
		}
	}

	for (int i = 1; i < strlen(str2); i++) {
		if (str2[i] == str1[0]) {
			end = i;
			break;
		}
	}

	printf("Start: %i\nEnd: %i\n", start, end);

	str1[start] = '\0';
	strncat(str1, str2, end+1);
	printf("Final: %s", str1);
}

int main() {
	//l4ex5();
	//l4ex6();
	l4ex8();
	return 0;
}