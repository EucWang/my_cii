//
// Created by wangxn on 2019/11/25.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "examples.h"
#include "array.h"
#include "arrayrep.h"

void set_array_reshape(){

    Array_T pT = Array_new(12, 1);

    for (int i = 0; i < Array_length(pT); ++i) {
        char i1 = i + 65;
        Array_put(pT, i, &i1);
    }
    
    
    for (int i = 0; i < Array_length(pT); ++i) {
        void *pVoid = Array_get(pT, i);
        printf("pT's element at %d is %c.\n", i, *(char *)pVoid);
    }

    Array_reshape(pT, 12, 4);
    printf("\nafter reshape(), then all value is.\n");
    for (int i = 0; i < Array_length(pT); ++i) {
        void *pVoid = Array_get(pT, i);
        printf("pT's element at %d.0 is %c.\n", i, *(char *)(pVoid));
        printf("pT's element at %d.1 is %c.\n", i, *(char *)(pVoid + 1));
        printf("pT's element at %d.2 is %c.\n", i, *(char *)(pVoid + 2));
        printf("pT's element at %d.3 is %c.\n", i, *(char *)(pVoid + 3));
    }

    Array_reshape(pT,12, 1);
    printf("\nafter reshape back, then all value is.\n");
    for (int i = 0; i < Array_length(pT); ++i) {
        void *pVoid = Array_get(pT, i);
        printf("pT's element at %d is %c.\n", i, *(char *)(pVoid));
    }

    Array_reshape(pT,4, 4);
    printf("\nafter reshape again , trim then all value is.\n");
    for (int i = 0; i < Array_length(pT); ++i) {
        void *pVoid = Array_get(pT, i);
        printf("pT's element at %d is %c.\n", i, *(char *)(pVoid));
        printf("pT's element at %d.1 is %c.\n", i, *(char *)(pVoid + 1));
        printf("pT's element at %d.2 is %c.\n", i, *(char *)(pVoid + 2));
        printf("pT's element at %d.3 is %c.\n", i, *(char *)(pVoid + 3));
    }

    Array_free(&pT);
}

void test_array(){

    Array_T pT = Array_new(10, 4);

    srand(time(NULL));

    for (int i = 0; i < Array_length(pT); ++i) {
        int i1 = rand() % 100;
//        i1 = abs(i1);
        if (i != 3) {
            Array_put(pT,i, &i1);
        }
    }

    for (int i = 0; i < Array_length(pT); ++i) {
        size_t *pVoid = (size_t *)Array_get(pT, i);
        printf("element at %d is %d.\n", i, *pVoid);
    }

    Array_T pArrayT = Array_copy(pT, 20);
    for (int i = 0; i < Array_length(pArrayT); ++i) {
        printf("pArrayT's element at %d is %d.\n", i, *(int *)Array_get(pArrayT, i));
    }

    Array_free(&pT);

    int size = Array_size(pArrayT);
    printf("pArrayT's size is %d.\n", size);


    Array_resize(pArrayT, 15);
    for (int i = 0; i < Array_length(pArrayT); ++i) {
        printf("pArrayT's element at %d is %d.\n", i, *(int *)Array_get(pArrayT, i));
    }

    Array_free(&pArrayT);

    printf("\n=============set_array_reshape()======\n");
    set_array_reshape();
}