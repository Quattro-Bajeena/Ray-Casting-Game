#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	char name[20];
	char surname[20];
	int index;
	char faculty[50];
	int year;
	int group;
} Student;


void PrintStudent(Student student) {
	printf("Name: %s %s\n", student.name, student.surname);
	printf("Index: %i\nYear: %i\nGroup: %i\n", student.index, student.year, student.group);
	printf("Faculty: %s\n", student.faculty);
	puts("------------");
}

void PrintStudents(Student* students, int length) {
	for (int i = 0; i < length; i++) {
		PrintStudent(students[i]);
	}
	
}



Student InputStudent() {
	char name[20], surname[20], faculty[50];
	int index, year, group;

	puts("Input: ");
	puts("Name Surname");
	scanf("%s %s", name, surname);
	puts("index year group");
	scanf("%i %i %i", &index, &year, &group);
	puts("faculty");
	scanf("%s", faculty);

	
	Student student;
	strcpy(student.name, name);
	strcpy(student.surname, surname);
	strcpy(student.faculty, faculty);
	student.index = index;
	student.year = year;
	student.group = group;

	return student;
}

void l3ex1() {

	Student me = InputStudent();
	PrintStudent(me);

}




int StudentCompare(const void* p1, const void* p2) {
	Student* student1 = (Student*)p1;
	Student* student2 = (Student*)p2;

	if (strcmp((*student1).surname, (*student2).surname) == 0) {
		return strcmp((*student1).name, (*student2).name);

	}
	else
		return strcmp((*student1).surname, (*student2).surname);
		
}

void l3ex2() {

	Student st1 = { "BMateusz", "Oleszek", 11111, "Inf", 1, 6 };
	Student st2 = { "ANieMateusz", "Oleszek", 22222, "Inf", 1, 6 };
	Student st3 = { "Joey", "AWheeler", 333333, "Inf", 1, 6 };
	Student st4 = { "Jean", "ZGiraud", 44444, "Inf", 1, 6 };

	Student students[15] = { st1, st2, st3, st4 };

	qsort(students, 4, sizeof(Student), StudentCompare);

	for (int i = 0; i < 4; i++) {
		PrintStudent(students[i]);
		
	}
}



void l3ex3() {
	int studentNum = 30;
	Student* students = (Student*)malloc(studentNum * sizeof(Student));

	Student st1 = { "Czesław", "Meyer", 11111, "Inf", 1, 6 };
	Student st2 = { "Huey", "Laforet", 22222, "Inf", 1, 6 };
	Student st3 = { "Joey", "Wheeler", 333333, "Inf", 1, 6 };
	Student st4 = { "Jean", "Giraud", 44444, "Inf", 1, 6 };
	Student st5 = { "Milia", "Harvent", 1111,"crm", 5, 6 };
	Student st6 = { "Claire", "Stanfield", 2222, "rtr", 5, 6 };
	Student st7 = { "Isac", "Dian", 3333,"geo", 2, 3 };
	Student st8 = { "Firo", "Prochainezo", 4444, "run", 7,8 };
	Student st9 = { "Luck ", "Gandor", 231, "Inf", 1, 6 };
	Student st10 = { "Nice", "Holystone", 12312, "Inf", 1, 6 };

	students[0] = st1; students[1] = st2; students[2] = st3; students[3] = st4; students[4] = st5;
	students[5] = st6; students[6] = st7; students[7] = st8; students[8] = st9; students[9] = st10;

	qsort(students, studentNum / 3, sizeof(Student), StudentCompare);

	for (int i = 0; i < studentNum/3; i++) {
		PrintStudent(students[i]);

	}

	free(students);
}

void l3ex4() {
	int studentNum = 30;
	Student* students = new Student[10];

	Student st1 = { "Czesław", "Meyer", 11111, "Inf", 1, 6 };
	Student st2 = { "Huey", "Laforet", 22222, "Inf", 1, 6 };
	Student st3 = { "Joey", "Wheeler", 333333, "Inf", 1, 6 };
	Student st4 = { "Jean", "Giraud", 44444, "Inf", 1, 6 };
	Student st5 = { "Milia", "Harvent", 1111,"crm", 5, 6 };
	Student st6 = { "Claire", "Stanfield", 2222, "rtr", 5, 6 };
	Student st7 = { "Isac", "Dian", 3333,"geo", 2, 3 };
	Student st8 = { "Firo", "Prochainezo", 4444, "run", 7,8 };
	Student st9 = { "Luck", "Gandor", 231, "Inf", 1, 6 };
	Student st10 = { "Chane", "Laforet", 12312, "Inf", 1, 6 };

	students[0] = st1; students[1] = st2; students[2] = st3; students[3] = st4; students[4] = st5;
	students[5] = st6; students[6] = st7; students[7] = st8; students[8] = st9; students[9] = st10;

	qsort(students, studentNum / 3, sizeof(Student), StudentCompare);

	for (int i = 0; i < studentNum / 3; i++) {
		PrintStudent(students[i]);

	}

	delete[] students;
}

void CreateTable1(Student* students, int length) {

	for (int i = 0; i < length; i++) {
		students[i] = InputStudent();
	}
}

Student* CreateTable2(int length) {
	Student* students = (Student*)malloc(length * sizeof(Student));

	for (int i = 0; i < length; i++) {
		students[i] = InputStudent();
	}
	return students;
}


int DeleteStudent(Student* students, int indexNumber, int length) {
	for (int i = 0; i < length; i++) {
		if (students[i].index == indexNumber) {
			
			for (int j = i; j < length - 1; j++) {
				students[j] = students[j + 1];
			}
			return length - 1;
		}

	}
	return length;
}

Student* InsertStudent(Student* students, Student studentToInsert, int* length) {
	Student* newStudents = (Student*)realloc(students, (*length + 1) * sizeof(Student));

	if (newStudents == NULL)
		return students;

	students[*length] = studentToInsert;
	students = newStudents;
	(*length)++;
	return students;
}

void SortStudents(Student* students, int length) {
	qsort(students, length, sizeof(Student), StudentCompare);
}

void l3ex5() {
	//5.1
	/*int len1 = 2;
	Student* students1 = (Student*)malloc(len1 * sizeof(Student));
	CreateTable1(students1, len1);
	PrintStudents(students1, len1);*/

	//5.2
	/*int len2 = 2;
	Student* students2 = CreateTable2(len2);
	PrintStudents(students2, len2);*/

	int len3 = 3;
	Student* students3 = (Student*)malloc(len3 * sizeof(Student));

	
	Student st1 = { "Milia", "Harvent", 1111,"crm", 5, 6 }; students3[0] = st1;
	Student st2 = { "Claire", "Stanfield", 2222, "rtr", 5, 6 }; students3[1] = st2;
	Student st3 = { "Isac", "Dian", 3333,"geo", 2, 3 }; students3[2] = st3;
	PrintStudents(students3, len3);
	puts("-------------");
	//5.3
	len3 = DeleteStudent(students3, 1111, len3);

	//5.4
	Student newStudent = { "Firo", "Prochainezo", 4444, "run", 7,8 };
	students3 = InsertStudent(students3, newStudent, &len3);
	PrintStudents(students3, len3);
	puts("-------------");

	//5.5
	SortStudents(students3, len3);

	PrintStudents(students3, len3);

	free(students3);

}

int _______main() {

	//W celu uproszczenia sprawdzania, w późniejszych zadaniach dane studentów są wpisane na stałe
	//Żeby nie trzeba było ich za każdym razem wpisywać

	//l3ex1();
	//l3ex2();
	//l3ex3();
	//l3ex4();
	l3ex5();
	return 0;
		

}