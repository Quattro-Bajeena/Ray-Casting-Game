#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include <string.h>
#include<math.h>
#include <stdbool.h>

void ex4() {
	float a, b, c;
	scanf("%f %f %f", &a, &b, &c);
	printf("%f, %f, %f\n", a, b, c);

	float delta = (b * b) - 4 * a * c;
	printf("%f\n", delta);

	if (delta == 0) {
		float root = -(b / a);
		printf("One root: %f\n", root);
	}
	else if (delta < 0) {
		printf("No roots\n");
	}
	else {
		float sum = -(b / a);
		printf("Two roots - sum: %f\n", sum);

	}
}



void ex6() {

	float a, b, c;
	scanf("%f %f %f", &a, &b, &c);
	printf("%f, %f, %f\n", a, b, c);


	float max = fmax(fmax(a, b), c);

	bool can_be = false;

	if (a == max) {
		can_be = b + c > a;
	}
	else if (b == max) {
		can_be = a + c > b;
	}
	else if (c == max) {
		can_be = a + b > c;
	}

	if (can_be == true) {
		printf("this can be a trinagle\n");
	}
	else {
		printf("this can't be a trinagle\n");
	}
	
}


void ex7() {

	float x;
	scanf("%f", &x);
	
	float numerator = (sin(pow(x, 2.f)) * log(15));
	float denominator = sqrt(fabs(log(0.5 * x) / log(3.f)));

	printf("%f \n %f\n", log(0.5 * x), log(3.f));

	printf("numerator: %f \ndenominator: %f\n", numerator, denominator);

	float number = numerator / denominator;
	printf("%f\n", number);

}


void ex10() {

	int size;
	char surname[100];
	//fgets(surname, 100, stdin);
	scanf("%s", surname);
	
	int i = 0;
	while (surname[i] != '\0') {

		char c = surname[i];
		//printf("%i ", (int)c);

		if ((int)c % 2 == 0) {
			printf("%c\n", c);
		}

		//printf("%c\n", surname[i]);
		i++;
	}
}

int _main() {    

	//ex6();
	//ex7();
	ex10();
	return 0; 
} 