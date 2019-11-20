//
// Created by wangxn on 2019/11/20.
//
#include <stdio.h>
#include <zconf.h>
#include "examples.h"
#include "table.h"

void printTableEnty(void * key, void ** value, void * cl) {
    printf("key is %d\t value is %d\n", key, *value);
}

void test_table(){

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
    printf("length is %d.\n", length);

    void *pVoid = Table_get(myTable, "abc7");

    printf("in myTable match the key 'abc7' and it's value is '%s'\n", pVoid);

    pVoid = Table_put(&myTable, "abc7", 1);

    printf("in myTable match the key 'abc7' and it's value is '%d'\n", pVoid);
    
    Table_free(&myTable);
    
    Table_T tableNew = Table_new(NULL, NULL);

    int * a1 = 1;
    int * a2 = 2;
    int * a3 = 3;
    int * a4 = 4;
    int * a5 = 5;
    int * a6 = 6;

    int * val1 = 12;
    int * val2 = 13;
    int * val3 = 14;

    int * val4 = 15;
    int * val5 = 16;
    int * val6 = 17;

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

    printf("in tableNew key1 the value is %d\n", get1);
    printf("in tableNew key2 the value is %d\n", get2);
    printf("in tableNew key3 the value is %d\n", get3);

    printf("in tableNew key4 the value is %d\n", get4);
    printf("in tableNew key5 the value is %d\n", get5);
    printf("in tableNew key6 the value is %d\n", get6);

    Table_map(tableNew, &printTableEnty, NULL);

}