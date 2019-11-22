#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include "atom.h"
#include "examples.h"
#include "set.h"



#define NHASH 29989
#define MULT 31
void logStrSet(void * member, void * cl){
    printf("the %s member of set is %s.\n", cl, member);
}

void logIntSet(void * member, void * cl){
    printf("the %s member of set is %d.\n", cl, *((int *)member));
}

void test_set_add(){
    Set_T myset = Set_new(100, NULL, NULL);

    int a1 = 100;
    int a2 = 110;
    int a3 = 120;
    int a4 = 130;
    int a5 = 140;
    int a6 = 150;
    int a7 = 160;
    int a8 = 170;
    int a9 = 180;
    int a10 = 190;
    int a11 = 200;
    int a12 = 300;
    int a13 = 400;
    int a14 = 500;
    int a15 = 600;

    Set_put(myset, &a1);
    Set_put(myset, &a2);
    Set_put(myset, &a3);
    Set_put(myset, &a4);
    Set_put(myset, &a5);
    Set_put(myset, &a6);
    Set_put(myset, &a7);
    Set_put(myset, &a8);
    Set_put(myset, &a9);
    Set_put(myset, &a9);
    Set_put(myset, &a9);
    Set_put(myset, &a9);

    Set_map(myset, logIntSet, "integer set");

    Set_free(&myset);
}

void test_set1(){
    
    Set_T myset = Set_new(100, NULL, NULL);

    Set_put(myset, "zhangsan");
    Set_put(myset, "zhangsan2");
    Set_put(myset, "zhangsan3");
    Set_put(myset, "zhangsan4");

    printf("set length is %d\n", Set_length(myset));

    if(Set_member(myset, "zhangsan4")){
        printf("'zhangsan4' is member of set.\n");
    } else {
        printf("'zhangsan4' is not member of set.\n");
    }

    if(Set_member(myset, "zhangsan")){
        printf("'zhangsan' is member of set.\n");
    } else {
        printf("'zhangsan' is not member of set.\n");
    }

    if(Set_member(myset, "zhangsan2")){
        printf("'zhangsan2' is member of set.\n");
    } else {
        printf("'zhangsan2' is not member of set.\n");
    }

    if(Set_member(myset, "zhangsan5")){
        printf("'zhangsan5' is member of set.\n");
    } else {
        printf("'zhangsan5' is not member of set.\n");
    }

    Set_map(myset, logStrSet, "amazing");

    void *pVoid = Set_remove(myset, "zhangsan2");
    printf("remove from set member is '%s'.\n", (char * )pVoid);
    printf("and then the length of set is %d.\n", Set_length(myset));
    Set_map(myset, logStrSet, "rest");

    void **array = Set_toArray(myset, "end");
    char * item;
    int i;
    printf("show the array member :\n");
    for(i =0, item = * array; item != NULL; item = *(array + i)) {
        logStrSet(item, "array");
        i++;
    }

    Set_free(&myset);
}



void test_union_set1(){
    Set_T s = Set_new(100, str_cmp, str_hash);

    char * str1 = "zhangsan1";
    char * str2 = "zhangsan2";
    char * str3 = "zhangsan3";
    char * str4 = "zhangsan4";

    char * str5 = "zhangsan5";
    char * str6 = "zhangsan6";
    char * str7 = "zhangsan7";
    char * str8 = "zhangsan8";

    char str9[] = {'z', 'h', 'a', 'n', 'g', 's', 'a', 'n', '4','\0'};

    Set_put(s, str1);
    Set_put(s, str2);
    Set_put(s, str3);
    Set_put(s, str4);

    Set_T t = Set_new(100, str_cmp, str_hash);

    Set_put(t, str5);
    Set_put(t, str6);
    Set_put(t, str7);
    Set_put(t, str9);

    Set_T union_set = Set_union(s, t);
    Set_map(union_set, logStrSet, "union set");
    printf("union set's length is %d.\n", Set_length(union_set));

}

void test_union_set2(){

    Set_T s1 = Set_new(100, int_cmp, int_hash);
    int  a1 = 1;
    int  a2 = 2;
    int  a3 = 3;
    int  a4 = 4;

    int  b1 = 4;
    int  b2 = 3;
    int  b3 = 6;
    int  b4 = 7;
    int  b5 = 8;

    Set_put(s1, &a1);
    Set_put(s1, &a2);
    Set_put(s1, &a3);
    Set_put(s1, &a4);

    Set_T t1 = Set_new(100, int_cmp, int_hash);

    Set_put(t1, &b1);
    Set_put(t1, &b2);
    Set_put(t1, &b3);
    Set_put(t1, &b4);
    Set_put(t1, &b5);

    Set_T union_set2 = Set_union(s1, t1);
    Set_map(union_set2, logIntSet, "union set2");
    printf("union set's length is %d.\n", Set_length(union_set2));
}

void test_set(){
//    test_set1();
//    test_set_add();
    test_union_set1();
    test_union_set2();
}