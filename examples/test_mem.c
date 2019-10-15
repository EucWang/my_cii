#include <stdio.h>
#include <string.h>
#include "mem.h"
#include "examples.h"

struct Student_ {
    char name[255];
    int age;
    int gender;
};
typedef struct Student_ Student;

struct HightSchool_Student_ {
    char name[255];
    int age;
    int gender;
    int howtail;
    int score;
};
typedef struct HightSchool_Student_ HighSchool_Student;

void test_mem(){

    Student * p;
    NEW(p);
    strcpy(p->name, "zhang san");
    p->age = 18;
    p->gender = 1;

    printf("Student's age is : %d\n", p->age);
    if (p->gender) {
        printf("Student's gender is : male\n");
    } else {
        printf("Student's gender is : female\n");
    }
    printf("Student's name is : %s\n", p->name);

    printf("---------------------------\n");


    Student * p1;
    NEW0(p1);
    strcpy(p1->name, "Stephen Zhou");
    p1->age = 18;
    p1->gender = 1;

    printf("Student2's age is : %d\n", p1->age);
    if (p1->gender) {
        printf("Student2's gender is : male\n");
    } else {
        printf("Student2's gender is : female\n");
    }
    printf("Student2's name is : %s\n", p1->name);

    printf("---------------------------\n");

    HighSchool_Student * hp2;
    RESIZE(p1, sizeof(HighSchool_Student));
    hp2 = p1;
    hp2->howtail = 180;
    hp2->score = 650;

    printf("HighSchool_Student's age is : %d\n", hp2->age);
    if (hp2->gender) {
        printf("HighSchool_Student's gender is : male\n");
    } else {
        printf("HighSchool_Student's gender is : female\n");
    }
    printf("HighSchool_Student's name is : %s\n", hp2->name);
    printf("HighSchool_Student's howtail is : %d\n", hp2->howtail);
    printf("HighSchool_Student's score is : %d\n", hp2->score);
    
    printf("---------------------------\n");
     printf("free Student1\n");
    FREE(p);
    printf("free Student2\n");
    FREE(p1);
    // printf("free Hight_School_Student3\n");
    // FREE(hp2);

    printf("---------------------------\n");
    Student * p3 = ALLOC(sizeof(Student));
    strcpy(p3->name, "zhang fei");
    p3->age = 34;
    p3->gender = 0;
    printf("Student3's name is : %s\n", p3->name);
    printf("Student3's gender is : %s\n", p3->gender?"male":"female");
    printf("Student3's age is %d\n", p3->age);
    printf("free Student3\n");
    FREE(p3);
}