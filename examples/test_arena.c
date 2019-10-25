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

void logArenaStatus(Arena_Status arenaStatus) {
    printf("\t\t----Arena Status :----\n");
    printf("\t\ttotal Memory : %ld\n", arenaStatus->totalMem);
    printf("\t\tused Memory : %ld, and percentage : %.01f%%\n", arenaStatus->usedMem, (arenaStatus->usedMem * 1.0/arenaStatus->totalMem)*100);
    printf("\t\tfree Memory : %ld\n", arenaStatus->freeMem);
    printf("\t\tblocks count : %d\n", arenaStatus->blocks);
    printf("\t\t--------------------\n");
}

void test_arena(){
    printf("test_arena() call.\n");

    Arena_T arena = Arena_new();
    logArenaStatus(Arena_status());
    ArenaTestSudent * b = (ArenaTestSudent *) Arena_alloc(arena, sizeof(ArenaTestSudent), __FILE__, __LINE__);
    strcpy(b->name, "zhang san");
    b->age = 18;
    b->gender = true;
    strcpy(b->grade, "grade seven");
    logStudent(b);
    logArenaStatus(Arena_status());
//
//    ArenaTestSudent * c = (ArenaTestSudent *) Arena_alloc(arena, sizeof(ArenaTestSudent), __FILE__, __LINE__);
//    strcpy(c->name, "stephen zhou");
//    c->age = 17;
//    c->gender = true;
//    strcpy(c->grade, "grade four");
//    logStudent(c);
//    ArenaTestSudent * d = (ArenaTestSudent *) Arena_alloc(arena, sizeof(ArenaTestSudent), __FILE__, __LINE__);
//    strcpy(d->name, "liubei");
//    d->age = 28;
//    d->gender = true;
//    strcpy(d->grade, "grade five");
//    logStudent(d);


//    int size = 9 * 1024;
    int count = 20;
    ArenaTestSudent * p = (ArenaTestSudent *)Arena_calloc(arena, count, sizeof(ArenaTestSudent), __FILE__, __LINE__);
    for (int i = 0; i < count; ++i) {
        strcpy((p + i)->name, "wahaha");
        (p+i)->age = (i+1)*2;
        (p+i)->gender = i%2 ? true : false;
        strcpy((p+i)->grade, "grade six six six");
        logStudent((p+i));
    }
    logArenaStatus(Arena_status());

    count = 19;
    ArenaTestSudent * p2 = (ArenaTestSudent *)Arena_calloc(arena, count, sizeof(ArenaTestSudent), __FILE__, __LINE__);
    for (int i = 0; i < count; ++i) {
        strcpy((p2 + i)->name, "wahaha");
        (p2+i)->age = (i+1)*2;
        (p2+i)->gender = i%2 ? true : false;
        strcpy((p2+i)->grade, "grade six six six");
        logStudent((p2+i));
    }
    logArenaStatus(Arena_status());

    ArenaTestSudent * d = (ArenaTestSudent *) Arena_alloc(arena, sizeof(ArenaTestSudent), __FILE__, __LINE__);
    strcpy(d->name, "liubei");
    d->age = 28;
    d->gender = true;
    strcpy(d->grade, "grade five");
    logStudent(d);
    logArenaStatus(Arena_status());

    ArenaTestSudent * e = (ArenaTestSudent *) Arena_alloc(arena, sizeof(ArenaTestSudent), __FILE__, __LINE__);
    strcpy(e->name, "wahaha");
    e->age = 13;
    e->gender = true;
    strcpy(e->grade, "grade six");
    logStudent(e);
    logArenaStatus(Arena_status());

//    char * p2 = (char *)Arena_calloc(arena, size, sizeof(char), __FILE__, __LINE__);

//    Arena_free(arena);

//    char * p3 = (char *)Arena_calloc(arena, size, sizeof(char), __FILE__, __LINE__);
//    char * p4 = (char *)Arena_calloc(arena, size, sizeof(char), __FILE__, __LINE__);

    Arena_dispose(arena);
}
