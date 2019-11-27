/**
 * Created by wangxn on 2019/11/27.
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ring.h"
#include "examples.h"


void test_ring(){
    
    Ring_T ring = Ring_new();
    
    Ring_addhigh(ring, "zhangsan");
    Ring_addhigh(ring, "lisi");
    Ring_addhigh(ring, "wangwu");
    Ring_addhigh(ring, "zhaoliu");
    Ring_addhigh(ring, "guanyu");
    Ring_addhigh(ring, "machao");

    printf("show all items of ring.\n");
    for (int i = 0; i < Ring_length(ring); ++i) {
        char *item = Ring_get(ring, i);
        printf("item at index %d of ring is %s.\n", i, item);
    }

    Ring_put(ring, 0, "liubei");
    Ring_put(ring, 5, "caocao");

    printf("after call Ring_put() then show all items of ring.\n");
    for (int i = 0; i < Ring_length(ring); ++i) {
        char *item = Ring_get(ring, i);
        printf("item at index %d of ring is %s.\n", i, item);
    }

    Ring_addlow(ring, "sunquan");
    Ring_addlow(ring, "sunce");
    Ring_addlow(ring, "zhouyu");

    printf("after call Ring_addlow() then show all items of ring.\n");
    for (int i = 0; i < Ring_length(ring); ++i) {
        char *item = Ring_get(ring, i);
        printf("item at index %d of ring is %s.\n", i, item);
    }

    Ring_add(ring, 3, "ma da ha");
    Ring_add(ring, -3, "da ma ha");
    Ring_add(ring, 1, "da da ma");
    printf("after call Ring_add() then show all items of ring.\n");
    for (int i = 0; i < Ring_length(ring); ++i) {
        char *item = Ring_get(ring, i);
        printf("item at index %d of ring is %s.\n", i, item);
    }

    for (int j = 0; j < 3; ++j) {
        Ring_remhigh(ring);
    }
    printf("after call Ring_remhigh() then show all items of ring.\n");
    for (int i = 0; i < Ring_length(ring); ++i) {
        char *item = Ring_get(ring, i);
        printf("item at index %d of ring is %s.\n", i, item);
    }

    for (int j = 0; j < 2; ++j) {
        Ring_remlow(ring);
    }
    printf("after call Ring_remlow() then show all items of ring.\n");
    for (int i = 0; i < Ring_length(ring); ++i) {
        char *item = Ring_get(ring, i);
        printf("item at index %d of ring is %s.\n", i, item);
    }

    for (int j = 0; j < 2; ++j) {
        Ring_remove(ring, 3);
    }
    printf("after call Ring_remove(3) twice then show all items of ring.\n");
    for (int i = 0; i < Ring_length(ring); ++i) {
        char *item = Ring_get(ring, i);
        printf("item at index %d of ring is %s.\n", i, item);
    }

    Ring_rotate(ring, Ring_length(ring));
    printf("after call Ring_rotate(length) then show all items of ring.\n");
    for (int i = 0; i < Ring_length(ring); ++i) {
        char *item = Ring_get(ring, i);
        printf("item at index %d of ring is %s.\n", i, item);
    }

    Ring_rotate(ring, Ring_length(ring)/2);
    printf("after call Ring_rotate(length/2) then show all items of ring.\n");
    for (int i = 0; i < Ring_length(ring); ++i) {
        char *item = Ring_get(ring, i);
        printf("item at index %d of ring is %s.\n", i, item);
    }

    Ring_free(&ring);


    Ring_T ring1 = Ring_ring("zhangfei", "gaunyu", "zhaoyun", "liubei", "machao", "huangzhong", NULL);
    printf("show all items of ring1.\n");
    for (int i = 0; i < Ring_length(ring1); ++i) {
        char *item = Ring_get(ring1, i);
        printf("item at index %d of ring1 is %s.\n", i, item);
    }

    Ring_free(ring1);
}
