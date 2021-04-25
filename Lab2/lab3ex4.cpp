//#define _CRT_SECURE_NO_WARNINGS
//#include <cstdio>
//#include <cstdlib>
//#include <cstring>
//
//struct Student {
//	char name[20];
//	char surname[20];
//	int index;
//	char faculty[50];
//	int year;
//	int group;
//} ;
//
//
//void PrintStudent(Student student) {
//	printf("Name: %s Surname: %s\n", student.name, student.surname);
//	printf("Index: %i Year: %i Group: %i\n", student.index, student.year, student.group);
//	printf("Faculty: %s\n", student.faculty);
//	puts("------------");
//}
//
//
//Student InputStudent() {
//	char name[20], surname[20], faculty[50];
//	int index, year, group;
//
//	puts("Name Surname");
//	scanf("%s %s", name, surname);
//	puts("index year group");
//	scanf("%i %i %i", &index, &year, &group);
//	puts("faculty");
//	scanf("%s", faculty);
//
//	Student student = {};
//	strcpy(student.name, name);
//	strcpy(student.surname, surname);
//	strcpy(student.faculty, faculty);
//	student.index = index;
//	student.year = year;
//	student.group = group;
//
//	return student;
//}
//
//int StudentCompare(const void* p1, const void* p2) {
//	Student* student1 = (Student*)p1;
//	Student* student2 = (Student*)p2;
//
//	if (strcmp((*student1).surname, (*student2).surname) == 0) {
//		return strcmp((*student1).name, (*student2).name);
//
//	}
//	else
//		return strcmp((*student1).surname, (*student2).surname);
//
//}





//int ___main() {
//
//
//
//	l3ex4();
//	return 0;
//
//
//}