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
    Table_T table = Table_new(NULL, NULL);  //生成一个新的Table
    char buf[128];

    while(getword(fp, buf, sizeof(buf), first, rest)) {  //循环遍历,获得文件中的每一个单词
        const char * word;

        int i;
        int * count;

        for(i = 0; buf[i] != '\0'; i++) {
            buf[i] = tolower(buf[i]);
        }
        word = Atom_string(buf);  //单词转换成小写之后,再转换成Atom类型

        count = Table_get(table, word);  //查看表中是否有这个单词
        if (count) {       //有,则单词数量+1
            (*count) ++;
        } else {
            NEW(count);  //否则创建int值,
            *count = 1;
            Table_put(&table, word, count);  //将单词和int值存入到table中
        }
    }

    if(name) {
        printf("%s:\n", name);  //这是文件名
    }

    //--print the words 90--
    int i;
    void ** array = Table_toArray(table, NULL);  //将table转换成数组
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
            wf(argv[i], fp);  //第一个参数是文件的路径
            fclose(fp);
        }
    }

//    if(argc == 1) {
//        wf(NULL, stdin);
//    }

    return EXIT_SUCCESS;
}