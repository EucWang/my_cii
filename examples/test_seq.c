//
// Created by wangxn on 2019/11/27.
//
#include "examples.h"
#include <stdio.h>
#include <stdlib.h>
#include <seq.h>

void test_seq3(){

    printf("\n========================\ntest_seq3() running...\n");

    int a1 = 1;
    int a2 = 2;
    int a3 = 3;
    int a4 = 4;
    int a5 = 5;
    int a6 = 6;
    int a7 = 7;
    int a8 = 8;
    int a9 = 9;
    int a10 = 10;

    Seq_T seqSeq = Seq_seq(&a1, &a2, &a3, &a4, &a5, &a6, &a7, NULL);

    for (int i = 0; i < Seq_length(seqSeq); ++i) {
        int *pVoid = Seq_get(seqSeq, i);
        printf("item at %d is : %d\n", i ,*pVoid);
    }

    int index = 3;
    int *put = Seq_put(seqSeq, index, &a10);
    printf("\talter item %d's value ,\n\tthe old value is %d, \n\tnew value is %d", index, *put, *(int *)Seq_get(seqSeq, index));

    printf("show all\n");
    for (int i = 0; i < Seq_length(seqSeq); ++i) {
        int *pVoid = Seq_get(seqSeq, i);
        printf("item at %d is : %d\n", i ,*pVoid);
    }

}

void test_seq2(){

    printf("\n========================\ntest_seq2() running...\n");

    Seq_T seqNew = Seq_new(4);
    
    char * a1 = "zhangsan";
    char * a2 = "lisi";
    char * a3 = "wangwu";
    char * a4 = "zhaoliu";
    char * a5 = "guanyu";
    char * a6 = "zhaoyun";
    char * a7 = "machao";
    char * a8 = "huangzhong";
    char * a9 = "zhangliao";
    char * a10 = "xiahouyuan";
    
    char * a11 = "liubei";
    char * a12 = "caocao";
    char * a13 = "sunquan";
    char * a14 = "sunce";
    char * a15 = "zhugeliang";
    char * a16 = "zhouyu";
    char * a17 = "haunggai";
    char * a18 = "huangzhong";
    char * a19 = "lusu";
    char * a20 = "caocao";
    
    Seq_addhigh(seqNew, a1);
    Seq_addhigh(seqNew, a2);
    Seq_addhigh(seqNew, a3);
    Seq_addhigh(seqNew, a4);
    Seq_addhigh(seqNew, a5);
    Seq_addhigh(seqNew, a6);
    Seq_addhigh(seqNew, a7);
    Seq_addhigh(seqNew, a8);
    Seq_addhigh(seqNew, a9);
    Seq_addhigh(seqNew, a10);

    Seq_addhigh(seqNew, a11);
    Seq_addhigh(seqNew, a12);
    Seq_addhigh(seqNew, a13);
    Seq_addhigh(seqNew, a14);
    Seq_addhigh(seqNew, a15);
    Seq_addhigh(seqNew, a16);
    Seq_addhigh(seqNew, a17);
    Seq_addhigh(seqNew, a18);
    Seq_addhigh(seqNew, a19);
    Seq_addhigh(seqNew, a20);

    for (int i = 0; i < Seq_length(seqNew); ++i) {
        char *pVoid = Seq_get(seqNew, i);
        printf("item at %d is : %s\n", i ,pVoid);
    }

    for (int i = 0; i < 4; ++i) {
        printf("remove from seq at high port is %s\n", Seq_remhigh(seqNew));
    }

    printf("after remove some item then rest items :\n");
    for (int i = 0; i < Seq_length(seqNew); ++i) {
        char *pVoid = Seq_get(seqNew, i);
        printf("item at %d is : %s\n", i ,pVoid);
    }

    Seq_free(&seqNew);
}

void test_seq1(){
    printf("\n========================\ntest_seq2() running...\n");

    Seq_T seqNew = Seq_new(12);

    char * a1 = "zhangsan";
    char * a2 = "lisi";
    char * a3 = "wangwu";
    char * a4 = "zhaoliu";
    char * a5 = "guanyu";
    char * a6 = "zhaoyun";
    char * a7 = "machao";
    char * a8 = "huangzhong";
    char * a9 = "zhangliao";
    char * a10 = "xiahouyuan";

    char * a11 = "liubei";
    char * a12 = "caocao";
    char * a13 = "sunquan";
    char * a14 = "sunce";
    char * a15 = "zhugeliang";
    char * a16 = "zhouyu";
    char * a17 = "haunggai";
    char * a18 = "huangzhong";
    char * a19 = "lusu";
    char * a20 = "caocao";

    Seq_addlow(seqNew, a1);
    Seq_addlow(seqNew, a2);
    Seq_addlow(seqNew, a3);
    Seq_addlow(seqNew, a4);
    Seq_addlow(seqNew, a5);
    Seq_addlow(seqNew, a6);
    Seq_addlow(seqNew, a7);
    Seq_addlow(seqNew, a8);
    Seq_addlow(seqNew, a9);
    Seq_addlow(seqNew, a10);

    Seq_addlow(seqNew, a11);
    Seq_addlow(seqNew, a12);
    Seq_addlow(seqNew, a13);
    Seq_addlow(seqNew, a14);
    Seq_addlow(seqNew, a15);
    Seq_addlow(seqNew, a16);
    Seq_addlow(seqNew, a17);
    Seq_addlow(seqNew, a18);
    Seq_addlow(seqNew, a19);
    Seq_addlow(seqNew, a20);

    for (int i = 0; i < Seq_length(seqNew); ++i) {
        char *pVoid = Seq_get(seqNew, i);
        printf("item at %d is : %s\n", i ,pVoid);
    }

    for (int i = 0; i < 5; ++i) {
        printf("remove from seq at low port is %s\n", Seq_remlow(seqNew));
    }

    printf("after remove some item then rest items :\n");
    for (int i = 0; i < Seq_length(seqNew); ++i) {
        char *pVoid = Seq_get(seqNew, i);
        printf("item at %d is : %s\n", i ,pVoid);
    }

    Seq_free(&seqNew);
}

void test_seq(){
    test_seq1();
    test_seq2();
    test_seq3();
}