#define _CRT_SECURE_NO_WARNINGS
#define M_PI 3.14159265358979323846
#include<stdio.h>
#include <string.h>
#include<math.h>
#include <stdbool.h>
#include <limits.h>
#include <time.h>


void ex1() {

    printf("%i\n%i\n%i\n%i\n%i\n%i\n%i\n%i\n%i\n%i\n",
        sizeof(bool), sizeof(char), 
        sizeof(int), sizeof(short), sizeof(long), 
        sizeof(float), sizeof(double),
        sizeof(long long), sizeof(unsigned int),
        sizeof(signed int)
    );
    
}

void ex2() {
    float number;
    scanf("%f", &number);

    printf("%f\n%.e\n%.3f\n%.3e",
        number, number, number, number);
}

void ex3() {

    int direction;
    scanf("%i", &direction);
    int number;
    scanf("%i", &number);


    if (direction == 16) {


    }
    else {

    }

}

void print_binary(unsigned int num) {
    int bi[32];
    for (int j = 0; j < 32; j++) // this for loop is initializing all the places with zeroes
    {
        bi[j] = 0;
    }
    int i = 31; // starting from the leftmost place of the array
    while (num > 0) //as the values in the array gets updated the remaining place is left with trailing 
    {
        if (num % 2 == 0)
            bi[i] = 0;
        else
            bi[i] = 1;
        i--;
        num = num / 2;
    } //zeroes
    for (int j = 0; j < 32; j++) {
        printf("%d", bi[j]);
    }
    printf("\n");
}

void ex4() {
    unsigned int alpha = 2810252828;

    //printf("%u", &alpha);
    //print_binary(alpha);
    //print_binary(alpha);
    bool pass = false;
    while (alpha > 0b1111) {
        alpha >>= 1;

        pass = alpha == 0b1011;
    }
    if (pass)
        printf("YES\n");
    else
        printf("NO\n");

}

void ex5() {
    unsigned int stan = 0xa7810e1c;
    unsigned int maska = 0x532a900c;

    unsigned int result = ~maska & stan;

    print_binary(stan);
    print_binary(maska);
    print_binary(result);

    printf("%#x\n", result);
}

void ex6() {

    for (int i = 0; i < 256; i++) {
        printf("%i %c\n", i, i);

        if (i % 40 == 0) {
            getch();
        }
    }

}

void ex7(){

    puts("Pick an exercise");
    int num;
    bool exercies_picked = true;

    do {
        scanf("%i", &num);

        switch (num)
        {
        
        case 1:
            ex1();
            break;
        case 2:
            ex2();
            break;
        case 3:
            ex3();
            break;
        case 4:
            ex4();
            break;
        case 5:
            ex5();
            break;
        case 6:
            ex6();
            break;
        default:
            puts("There is not exercies with this number");
            exercies_picked = false;
            break;
        }

    } while (exercies_picked == false);

    puts("Congratulations");

}

void ex8() {

    puts("1. add");
    puts("2. subtract");
    puts("3. multiply");
    puts("4. divide");

    int option, num1, num2;
    float result;
    scanf("%i", &option);

    puts("Enter first number");
    scanf("%i", &num1);
    puts("Enter second number");
    scanf("%i", &num2);


    switch (option)
    {
    case 1:
        result = num1 + num2;
        break;
    case 2:
        result = num1 - num2;
        break;
    case 3:
        result = num1 * num2;
        break;
    case 4:
        result = (float)num1 / num2;
        break;

    default:
        puts("Picked wrong option");
        return;
        break;
    }

    printf("Result: %f", result);

}

void ex9() {

    for (int i = 1; i <= 500; i++) {
        if (i % 7 == 0) {
            printf("%i ", i);
        }
    }

}

void ex10() {

    double answer = 0;
    for (int i = 1; i <= 10000; i++) {
        answer += 1. / pow(i, 2);
    }

    printf("%f\n", answer);

    double answer2 = 0;
    for (int i = 10000; i >= 1; i--) {
        answer2 += 1. / pow(i, 2);
    }

    printf("%f\n", answer2);

    double differnce = answer - answer2;
    printf("%f", differnce); 


}

void ex11() {
    double result = 0;

    puts("start");
    for (int n = 0; n < 10000000; n++) {
        result += pow(-1, n) / ((double)2 * n + 1);
    }

    result *= 4;
    printf("%Lf", result);
    //0.57

}


void ex12() {

    bool can_construct = false;
    float side1, side2, side3;
    side1 = 5;
    side2 = 10;
    side3 = 13;

    float longest_side = fmax(side1, fmax(side2, side3));
    
    if (longest_side == side1) {
        can_construct = side2 + side3 > side1;
    }
    else if (longest_side == side2) {
        can_construct = side1 + side3 > side2;
    }
    else if (longest_side == side3) {
        can_construct = side1 + side2 > side3;
    }

    if (can_construct == true) {
        puts("Can construct");
        float p = (side1 + side2 + side3) / 2;
        float area = sqrt(p * (p - side1) * (p - side2) * (p - side3));
        printf("Area: %f", area);
    }
    else {
        puts("Can't construct");
    }

}

void ex13() {
    float radians = M_PI;
    float degrees = radians * (180. / M_PI);
    printf("Degrees: %f", degrees);

}

void ChangeCase(char* str) {
    /*
    Each lowercase letter is 32 + uppercase equivalent.
    This means simply flipping the bit at position 5 (counting from least significant
    bit at position 0)
    inverts the case of a letter.
    */

    for (int i = 0; i < strlen(str); i++) {

        if ((str[i] > 64) && (str[i] < 91) || (str[i] > 96) && (str[i] < 123)) {
            str[i] ^= 0x20;
        }
    }


}

void ex14() {
    char str[] = "ALA ma 2 koty oraz 1 psa.";

    ChangeCase(str);

    puts(str);

}

void ex15() {

    char str[] = "kajak";
    int str_len = strlen(str);
    char* reversed = (char*)malloc(sizeof(str));
    

    for (int i = 0; i < str_len; i++) {

        //printf("%c %i\n", str[str_len - 1 - i], str[str_len - 1 - i]);

        reversed[i] = str[str_len - 1 - i];
    }
    reversed[str_len] = '\0';

    //puts(str);
    //puts(reversed);


    if (strcmp(str, reversed) == 0) {
        puts("Is palindrome");

        int half_len = (str_len + 1) / 2;
        char half[100];

        sprintf(half, "%.*s", half_len, str);
        printf("Half: %s\n", half);
    }
    else {
        puts("Isnt palindrome");
    }

    

}

bool czyTrojkat(float a, float b, float c) {
    bool can_construct = 0;

    float longest_side = fmax(a, fmax(b, c));

    if (longest_side == a) {
        can_construct = b + c > a;
    }
    else if (longest_side == b) {
        can_construct = a + c > b;
    }
    else if (longest_side == c) {
        can_construct = a + b > c;
    }

    return can_construct;
}

float obliczPole(float a, float b, float c){
    float p = (a + b + c) / 2;
    return sqrt(p * (p - a) * (p - b) * (p - c));
}

ex16() {
    int a = 5, b = 10, c = 20;
    bool is_triangle = czyTrojkat(a, b, c);

    if (is_triangle) {
        puts("Can construct triangle");
        float area = obliczPole(a, b, c);
        printf("Area: %f", area);
    }
    else {
        puts("Can't construct triangle");
    }


}

float naStopien(float rad) {
    return rad * (180. / M_PI);;
}

void ex17() {
    printf("%f", naStopien(M_PI));
}

bool czyPalindrom(const char str[]) {
    int str_len = strlen(str);
    char* reversed = (char*)malloc(sizeof(str));


    for (int i = 0; i < str_len; i++) {

        //printf("%c %i\n", str[str_len - 1 - i], str[str_len - 1 - i]);

        reversed[i] = str[str_len - 1 - i];
    }
    reversed[str_len] = '\0';

    printf("Org: %s Reversed: %s\n", str, reversed);

    return strcmp(str, reversed) == 0;
}

char* polPalindromu(const char str[]) {
    int str_len = strlen(str);
    int half_len = (str_len + 1) / 2;

    char* half = malloc(str_len / 2);
    

    sprintf(half, "%.*s", half_len, str);
    return half;
}

void ex18() {
    char str[] = "kaakj";
    bool is_palindrome = czyPalindrom(str);

    if (is_palindrome) {
        char* half = polPalindromu(str);
        puts("Is palindrome");
        printf("Half: %s\n", half);
    }
    else {
        puts("Isn't palindrome");
    }
}

int iterFibonacci(int n) {
    int t1 = 0, t2 = 1, next = 0;

    for (int i = 2; i <= n; i++) {
        next = t1 + t2;
        t1 = t2;
        t2 = next;
        
    }

    return t2;
}

int rekurFibonacci(int n) {
    //if (n > 1)
    //    return rekurFibonacci(n - 1) + rekurFibonacci(n - 2);
    //else
    //    return n;

    return n > 1 ? rekurFibonacci(n - 1) + rekurFibonacci(n - 2) : n;
    
}

void ex19() {

    int n = 35;
    clock_t start_iter = clock();
    int value1 = iterFibonacci(n);
    clock_t end_iter = clock();
    int value2 = rekurFibonacci(n);
    clock_t end_rekur = clock();

    double time_iter = (double)(end_iter - start_iter) / CLOCKS_PER_SEC;
    double time_rekur = (double)(end_rekur - end_iter) / CLOCKS_PER_SEC;

    printf("Iter: %i : %.20lfs\n", value1, time_iter);
    printf("Rekur: %i : %.20lfs\n", value2, time_rekur);

}

unsigned long long factorial(int number) {
    long result = 1;

    for (int c = 1; c <= number; c++) {
        result = result * c;
    }

    return result;
}

int digit_count(int number) {
    int count = 0;
    do {
        count++;
        number /= 10;
    } while (number != 0);
    return count;
}

char* string_filled(int length, char fill) {
    char* string = malloc(length + 1);
    for (int i = 0; i < length; i++) {
        string[i] = fill;
    }
    string[length] = '\0';
    return string;
}

char* pad_string(char* string, int target_length) {
    char* padding = string_filled(target_length, ' ');
    int pad_len = (target_length - strlen(string))/2;
    if (pad_len < 0) pad_len = 0;
    char* new_string = malloc(target_length + 1);
    sprintf(new_string, "%*.*s%s", pad_len, pad_len, padding, string);
    return new_string;

}

void ex20() {

    int height = 12;

    
    for (int n = 0; n < height; n++) {
        char* row = "";

        for (int r = 0; r <= n; r++) {
            
            unsigned long long a = factorial(n) / (factorial(r) * factorial(n - r));
            int digits = digit_count(a);
            char* num_str = malloc(digits + 2);
            sprintf(num_str, "%llu ", a);

            char* new_row = malloc(strlen(row) + strlen(num_str)+1);
            strcpy(new_row, row);
            strcat(new_row, num_str);

            
            row = new_row;
            
        }
        
        row = pad_string(row, 50);
        puts(row);
    }

    

}

int l2main()
{
    ex14();
    return 0;

}

