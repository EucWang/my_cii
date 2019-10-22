//
// Created by wangxn on 2019/10/22.
//
#include <stdio.h>
#include <stdbool.h>
#include <memory.h>
#include "arena.h"
#include "examples.h"

struct _Arena_Test_Student{
    char name[256];
    int age;
    bool gender;
    char grade[256];
};
typedef struct _Arena_Test_Student ArenaTestSudent;

void logStudent(ArenaTestSudent * testSudent) {
    printf("\n--------------------\n");
    printf("student name: %s\n", testSudent->name);
    printf("student age: %d\n", testSudent->age);
    if(testSudent->gender){
        printf("student gender: male\n");
    } else {
        printf("student gender: female\n");
    }

    printf("student grade: %s\n", testSudent->grade);
    printf("\n--------------------\n");
}


void test_arena(){
    printf("test_arena() call.");

    Arena_T arena = Arena_new();
    ArenaTestSudent * b = (ArenaTestSudent *) Arena_alloc(arena, sizeof(ArenaTestSudent), __FILE__, __LINE__);
    strcpy(b->name, "zhang san");
    b->age = 18;
    b->gender = true;
    strcpy(b->grade, "grade seven");
    logStudent(b);

    ArenaTestSudent * c = (ArenaTestSudent *) Arena_alloc(arena, sizeof(ArenaTestSudent), __FILE__, __LINE__);
    strcpy(c->name, "stephen zhou");
    c->age = 17;
    c->gender = true;
    strcpy(c->grade, "grade four");
    logStudent(c);
    ArenaTestSudent * d = (ArenaTestSudent *) Arena_alloc(arena, sizeof(ArenaTestSudent), __FILE__, __LINE__);
    strcpy(d->name, "liubei");
    d->age = 28;
    d->gender = true;
    strcpy(d->grade, "grade five");
    logStudent(d);
    ArenaTestSudent * e = (ArenaTestSudent *) Arena_alloc(arena, sizeof(ArenaTestSudent), __FILE__, __LINE__);
    strcpy(e->name, "wahaha");
    e->age = 13;
    e->gender = true;
    strcpy(e->grade, "grade six");
    logStudent(e);

    int size = 9 * 1024;
    char * p = (char *)Arena_calloc(arena, size, sizeof(char), __FILE__, __LINE__);
    char * p2 = (char *)Arena_calloc(arena, size, sizeof(char), __FILE__, __LINE__);

    Arena_free(arena);

    char * p3 = (char *)Arena_calloc(arena, size, sizeof(char), __FILE__, __LINE__);
    char * p4 = (char *)Arena_calloc(arena, size, sizeof(char), __FILE__, __LINE__);

    Arena_dispose(arena);
}
