/**
 * Created by wangxn on 2019/12/13.
*/

#include <stdio.h>
#include "examples.h"
#include "bit.h"
#include "mem.h"

void print_bit(int n, int bit, void * cl) {
    printf("%d", bit);
}

void test_bit1(){
    printf("test_bit1()\n");
    Bit_T bit1 = Bit_new(32);

    int length = Bit_length(bit1);
    printf("bit1's length is %d\n", length);


    int count = Bit_count(bit1);
    printf("bit1's count is %d\n", count);

    Bit_set(bit1, 0, 5);
    count = Bit_count(bit1);
    printf("after Bit_set(0, 5) then bit1's count is %d\n", count);

    int bitGet = Bit_get(bit1, 5);
    printf("BIt_get(5) bit value is %d\n", bitGet);

    int i = Bit_put(bit1, 5, 0);
    bitGet = Bit_get(bit1, 5);
    printf("old value is %d, and then BIt_get(5) bit value is %d\n", i , bitGet);

    count = Bit_count(bit1);
    printf("after Bit_put() then count is %d.\n", count);

    Bit_clear(bit1, 1, 3);
    count = Bit_count(bit1);
    printf("after Bit_clear(1, 3) then count is %d.\n", count);

    Bit_not(bit1, 0, 0);
    count = Bit_count(bit1);
    printf("after Bit_not(0, 0) then count is %d.\n", count);

    Bit_not(bit1, 1,2);
    count = Bit_count(bit1);
    printf("after Bit_not(1, 2) then count is %d.\n", count);
    Bit_free(&bit1);
}

void test_bit2(){
    printf("test_bit2()\n");
    Bit_T bit1 = Bit_new(32);
    Bit_T bit2 = Bit_new(32);
    Bit_T bit3 = Bit_new(32);

    Bit_set(bit1, 1, 20);
    Bit_set(bit2, 10, 20);
    Bit_set(bit3, 10, 30);

    printf("bit1:\t");
    Bit_map(bit1, print_bit, NULL);
    printf("\nbit2:\t");
    Bit_map(bit2, print_bit, NULL);
    printf("\nbit3:\t");
    Bit_map(bit3, print_bit, NULL);
    printf("\n");

    if(Bit_lt(bit2, bit1)){
        printf("bit2 is sub collection of bit1.\n");
    }
    if(Bit_lt(bit3, bit1)){
        printf("bit3 is sub collection of bit1.\n");
    } else {
        printf("bit3 is not sub collection of bit1.\n");
    }

    Bit_clear(bit3, 21, 30);
    printf("Bit_clear(bit3, 21, 30)\n");

    printf("bit1:\t");
    Bit_map(bit1, print_bit, NULL);
    printf("\nbit3:\t");
    Bit_map(bit3, print_bit, NULL);
    printf("\n");

    if(Bit_lt(bit3, bit1)) {
        printf("bit3 is letter than bit1.\n");
    } else {
        printf("bit3 is not letter than bit1.\n");
    }

    Bit_clear(bit1, 1, 9);
    printf("Bit_clear(bit1, 1, 9)\n");

    printf("bit1:\t");
    Bit_map(bit1, print_bit, NULL);
    printf("\nbit3:\t");
    Bit_map(bit3, print_bit, NULL);
    printf("\n");

    if(Bit_eq(bit1, bit3)) {
        printf("bit3 is equal to bit1.\n");
    } else {
        printf("bit3 is not equal to bit1.\n");
    }

    if(Bit_leq(bit3, bit1)) {
        printf("bit1 is less or equal to bit3.\n");
    } else {
        printf("bit1 is not less or equal to bit3.\n");
    }

    Bit_free(&bit1);
    Bit_free(&bit2);
    Bit_free(&bit3);
}

void test_bit3(){

    printf("test_bit2()\n");
    Bit_T bit1 = Bit_new(120);
    Bit_T bit2 = Bit_new(120);
    Bit_T bit3 = Bit_new(120);

    Bit_set(bit1, 60, 80);
    Bit_set(bit2, 50, 90);
    Bit_set(bit3, 40, 119);


    printf("bit1:\t");
    Bit_map(bit1, print_bit, NULL);
    printf("\nbit2:\t");
    Bit_map(bit2, print_bit, NULL);
    printf("\nbit3:\t");
    Bit_map(bit3, print_bit, NULL);
    printf("\n");

    if(Bit_leq(bit1, bit3)){
        printf("bit1 is leq to bit3\n");
    } else {
        printf("bit1 is not leq to bit3\n");
    }

    if(Bit_lt(bit1, bit3)){
        printf("bit1 is lt to bit3\n");
    } else {
        printf("bit1 is not lt to bit3\n");
    }

    if(Bit_eq(bit1, bit3)){
        printf("bit1 is eq to bit3\n");
    } else {
        printf("bit1 is not eq to bit3\n");
    }

    if(Bit_leq(bit2, bit3)){
        printf("bit2 is leq to bit3\n");
    } else {
        printf("bit2 is not leq to bit3\n");
    }

    if(Bit_lt(bit2, bit3)){
        printf("bit2 is lt to bit3\n");
    } else {
        printf("bit2 is not lt to bit3\n");
    }

    if(Bit_eq(bit2, bit3)){
        printf("bit2 is eq to bit3\n");
    } else {
        printf("bit2 is not eq to bit3\n");
    }

    Bit_set(bit2, 45, 49);
    printf("Bit_set(bit2, 45, 49)\n");

    printf("bit1:\t");
    Bit_map(bit1, print_bit, NULL);
    printf("\nbit2:\t");
    Bit_map(bit2, print_bit, NULL);
    printf("\nbit3:\t");
    Bit_map(bit3, print_bit, NULL);
    printf("\n");

    if(Bit_leq(bit2, bit3)){
        printf("bit2 is leq to bit3\n");
    } else {
        printf("bit2 is not leq to bit3\n");
    }

    Bit_set(bit1, 45, 59);
    Bit_set(bit1, 81, 90);
    printf("after Bit_set(bit1, 45-59, 81-90)\n");
    printf("bit1:\t");
    Bit_map(bit1, print_bit, NULL);
    printf("\nbit2:\t");
    Bit_map(bit2, print_bit, NULL);
    printf("\nbit3:\t");
    Bit_map(bit3, print_bit, NULL);
    printf("\n");

    if(Bit_leq(bit1, bit2)){
        printf("bit1 is leq to bit2\n");
    } else {
        printf("bit1 is not leq to bit2\n");
    }

    if(Bit_lt(bit1, bit2)){
        printf("bit1 is lt to bit2\n");
    } else {
        printf("bit1 is not lt to bit2\n");
    }

    if(Bit_eq(bit1, bit2)){
        printf("bit1 is eq to bit2\n");
    } else {
        printf("bit1 is not eq to bit2\n");
    }

    Bit_free(&bit1);
    Bit_free(&bit2);
    Bit_free(&bit3);
}

void test_bit4(){

    printf("test_bit4()\n");
    Bit_T bit1 = Bit_new(100);
    Bit_T bit2 = Bit_new(100);
    Bit_T bit3 = Bit_new(100);
    Bit_T bit4 = Bit_new(100);

    Bit_set(bit1, 60, 80);
    Bit_set(bit2, 50, 90);
    Bit_set(bit3, 20, 49);
    Bit_set(bit4, 70, 90);

    printf("bit1:\t");
    Bit_map(bit1, print_bit, NULL);
    printf("\nbit2:\t");
    Bit_map(bit2, print_bit, NULL);
    printf("\nbit3:\t");
    Bit_map(bit3, print_bit, NULL);
    printf("\nbit4:\t");
    Bit_map(bit4, print_bit, NULL);
    printf("\n");

    Bit_T bit5 = Bit_union(bit1, bit3);
    printf("Bit_union(bit1, bit3) then bit5 is result\n");
    printf("bit5:\t");
    Bit_map(bit5, print_bit, NULL);
    printf("\n");

    Bit_T bit6 = Bit_inter(bit1, bit2);
    printf("Bit_inter(bit1, bit2) then bit6 is result:\n");
    printf("bit6:\t");
    Bit_map(bit6, print_bit, NULL);
    printf("\n");

    Bit_T bit7 = Bit_minus(bit2, bit1);
    printf("Bit_minus(bit2, bit1) then bit7 is result:\n");
    printf("bit7:\t");
    Bit_map(bit7, print_bit, NULL);
    printf("\n");

    Bit_T bit8 = Bit_minus(bit1, bit2);
    printf("Bit_minus(bit1, bit2) then bit8 is result:\n");
    printf("bit8:\t");
    Bit_map(bit8, print_bit, NULL);
    printf("\n");

    Bit_T bit9 = Bit_diff(bit1, bit4);
    printf("Bit_diff(bit1, bit4) then bit9 is result:\n");
    printf("bit9:\t");
    Bit_map(bit9, print_bit, NULL);
    printf("\n");

    Bit_T bit10 = Bit_diff(bit3, bit4);
    printf("Bit_diff(bit3, bit4) then bit10 is result:\n");
    printf("bit10:\t");
    Bit_map(bit10, print_bit, NULL);
    printf("\n");

    Bit_free(&bit1);
    Bit_free(&bit2);
    Bit_free(&bit3);
    Bit_free(&bit4);
}

void test_bit(){
    test_bit4();
}