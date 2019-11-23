//
// Created by wangxn on 2019/11/20.
//
#include <stdio.h>
#include "examples.h"
#include "table.h"
#include "atom.h"

void printTableEnty(void * key, void ** value, void * cl) {
    printf("key is %d\t value is %d\n", *(int *)key, **((int **)value));
}

void printTableEnty2(void * key, void ** value, void * cl) {
    printf("key is %s\t value is %d\n", (char *)key, **((int **)value));
}


void test_table3(){

    Table_T tableNew = Table_new(str_cmp, str_hash);
    char a[] = {'a','b','c'};
    char * b = "abc";

    int arr[] = {1,2,3,4,5,6};

    int * a1 = &arr[0];
    int * a2 = &arr[1];
    int * a3 = &arr[2];

    Table_put(&tableNew, a, a1);
    Table_put(&tableNew, b, a2);
    Table_put(&tableNew, "abcd", a2);
    Table_put(&tableNew, "abcd", a3);

    printf("tableNew's length is %d\n", Table_length(tableNew));
    Table_map(tableNew, printTableEnty2, NULL);

    Table_free(&tableNew);
}

void test_table2(){

    Table_T tableNew = Table_new(int_cmp, int_hash);

    int arr[] = {1,2,3,4,5,6};
    int arr2[] = {12,13,14,15,16,17};

    int * a1 = &arr[0];
    int * a2 = &arr[1];
    int * a3 = &arr[2];

    int * a4 = &arr[3];
    int * a5 = &arr[4];
    int * a6 = &arr[5];

    int * val1 = &arr2[0];
    int * val2 = &arr2[1];
    int * val3 = &arr2[2];

    int * val4 = &arr2[3];
    int * val5 = &arr2[4];
    int * val6 = &arr2[5];

    Table_put(&tableNew, a1, val1);
    Table_put(&tableNew, a2, val2);
    Table_put(&tableNew, a3, val3);

    Table_put(&tableNew, a4, val4);
    Table_put(&tableNew, a5, val5);
    Table_put(&tableNew, a6, val6);

    int tableLength = Table_length(tableNew);
    printf("tableNew length is %d.\n", tableLength);
    void *get1 = Table_get(tableNew, a1);
    void *get2 = Table_get(tableNew, a2);
    void *get3 = Table_get(tableNew, a3);
    void *get4 = Table_get(tableNew, a4);
    void *get5 = Table_get(tableNew, a5);
    void *get6 = Table_get(tableNew, a6);

    printf("in tableNew key1 the value is %d\n", *(int *)get1);
    printf("in tableNew key2 the value is %d\n", *(int *)get2);
    printf("in tableNew key3 the value is %d\n", *(int *)get3);

    printf("in tableNew key4 the value is %d\n", *(int *)get4);
    printf("in tableNew key5 the value is %d\n", *(int *)get5);
    printf("in tableNew key6 the value is %d\n", *(int *)get6);

    Table_map(tableNew, &printTableEnty, NULL);

    Table_free(&tableNew);
}

void test_table1(){

    Table_T myTable = Table_new(NULL, NULL);

    Table_put(&myTable, "abc1", "zhangsan");
    Table_put(&myTable, "abc2", "zhangsan2");
    Table_put(&myTable, "abc3", "zhangsan3");

    Table_put(&myTable, "abc4", "zhangsan4");
    Table_put(&myTable, "abc5", "zhangsan5");
    Table_put(&myTable, "abc6", "zhangsan6");

    Table_put(&myTable, "abc7", "zhangsan6");
    Table_put(&myTable, "abc7", "zhangsan8");

    int length = Table_length(myTable);
    printf("myTable's length is %d.\n", length);

    void *pVoid = Table_get(myTable, "abc1");
    printf("in myTable match the key 'abc1' and it's value is '%s'\n", (char *)pVoid);

    pVoid = Table_get(myTable, "abc2");
    printf("in myTable match the key 'abc2' and it's value is '%s'\n", (char *)pVoid);

    pVoid = Table_get(myTable, "abc3");
    printf("in myTable match the key 'abc3' and it's value is '%s'\n", (char *)pVoid);

    pVoid = Table_get(myTable, "abc4");
    printf("in myTable match the key 'abc4' and it's value is '%s'\n", (char *)pVoid);

    pVoid = Table_get(myTable, "abc5");
    printf("in myTable match the key 'abc5' and it's value is '%s'\n", (char *)pVoid);

    pVoid = Table_get(myTable, "abc6");
    printf("in myTable match the key 'abc6' and it's value is '%s'\n", (char *)pVoid);

    pVoid = Table_get(myTable, "abc7");
    printf("in myTable match the key 'abc7' and it's value is '%s'\n", (char *)pVoid);

    int cc  = 100;
    Table_put(&myTable, "abc7", &cc);

    pVoid = Table_get(myTable, "abc7");

    printf("after change abc7 and then in myTable match the key 'abc7' and it's value is '%d'\n", *((int *)pVoid));

    Table_free(&myTable);

}

void test_table(){

    printf("\n\n------------test_table1------------\n\n");
    test_table1();

    printf("\n\n------------test_table2------------\n\n");
    test_table2();

    printf("\n\n------------test_table3------------\n\n");
    test_table3();
}