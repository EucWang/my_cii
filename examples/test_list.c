//
// Created by wangxn on 2019/10/28.
//

#include "list.h"
#include "examples.h"

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

void show_ele(void ** ele, void * cl) {
    printf("`%s`\t", *ele);
}

void test_list() {
    printf("test_list() calling...\n");
    List_T pt = List_list("zhangsan", NULL);
    pt = List_push(pt, "li si");  //必须重新赋值回原来的list
    pt = List_push(pt, "wang wu");
    pt = List_push(pt, "zhao liu");
    pt = List_push(pt, "zhou qi");
    printf("list length is %d\n", List_length(pt));

    List_T pt2 = List_list("张飞", "关羽", "刘备");
    printf("list2 length is %d\n", List_length(pt2));

    pt = List_append(pt, pt2);
    printf("after List_append() then \n");
    printf("list length is %d\n", List_length(pt));

    printf("show every element\n[");
    List_map(pt, show_ele, NULL);
    printf("]\n================\n");

    pt = List_reverse(pt);
    printf("after List_reverse() then show every element\n[");
    List_map(pt, show_ele, NULL);
    printf("]\n================\n");

    char a1 = 'a';
    char * a = &a1;
    char ** a2 = &a;
    pt = List_pop(pt, a2);
    printf("after List_pop() then value is `%s`\n", *a2);

    List_T ptc = List_copy(pt);
    printf("after List_copy() then show every element\n[");
    List_map(pt, show_ele, NULL);
    printf("]\n================\n");

    void ** pVoid = List_toArray(ptc, NULL);
    int length = List_length(ptc);
    printf("after List_toArray() then show Array values : \n[");
    for(int i=0; i < length; i++) {
        printf("`%s` ", *(pVoid + i));
    }
    printf("]\n-------------------------\n");

    List_free(ptc);
    List_free(pt);
    printf("after List_free(), free List without data.\n");
}
