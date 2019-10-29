//
// Created by wangxn on 2019/10/29.
//

#include <examples.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include "atom.h"
#include "mem.h"
#include "table.h"
#include "getword.h"

/**
 * 判断是不是单词除了首字母剩余的部分
 * @param c
 * @return
 */
static int rest(int c) {
    return isalpha(c) || c=='_';
}

/**
 * 判断是不是首字母
 * @param c
 * @return
 */
static int first(int c) {
    return isalpha(c);
}

/**
 * 默认的比较函数,调用字符串的比较函数
 * @param x
 * @param y
 * @return
 */
static int compare(const void * x, const void * y) {
    return strcmp(*(char **)x, *(char ** )y);
}

void vfree(const void *key, void **count, void * cl) {
    FREE(*count);
}

/**
 * 测试getword()方法
 * @param name
 * @param fp
 */
static void wf(char * name, FILE * fp) {
    Table_T table = Table_new(0, NULL, NULL);
    char buf[128];

    while(getword(fp, buf, sizeof(buf), first, rest)) {
        const char * word;

        int i;
        int * count;

        for(i = 0; buf[i] != '\0'; i++) {
            buf[i] = tolower(buf[i]);
        }
        word = Atom_string(buf);

        count = Table_get(table, word);
        if (count) {
            (*count) ++;
        } else {
            NEW(count);
            *count = 1;
            Table_put(table, word, count);
        }
    }

    if(name) {
        printf("%s:\n", name);
    }

    //--print the words 90--
    int i;
    void ** array = Table_toArray(table, NULL);
    qsort(array, Table_length(table), 2 * sizeof(*array), compare);  //排序

    for (i = 0;  array[i] ; i += 2) {
        printf("%d\t%s\n", *(int *) array[i+1], (char *)array[i]);
    }
    FREE(array);   //释放创建的数组

    //--deallocate the entries and table 91--
    Table_map(table, vfree, NULL);  //释放表中的数据
    Table_free(&table);                 //释放表

}

/**
 * 测试用例
 * @param argc
 * @param argv
 * @return
 */
int test_getword(int argc, char * argv[]) {
    int i;

    for (i = 1; i < argc ; i++) {
        FILE * fp = fopen(argv[i], "r");
        if (fp == NULL) {
            fprintf(stderr, "%s: can't open '%s' (%s)\n", argv[0], argv[i], strerror(errno));
            return EXIT_FAILURE;
        } else {
            wf(argv[i], fp);
            fclose(fp);
        }
    }

//    if(argc == 1) {
//        wf(NULL, stdin);
//    }

    return EXIT_SUCCESS;
}