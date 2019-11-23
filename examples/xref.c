//
// Created by wangxn on 2019/11/21.
//

// xref includes 103
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include "atom.h"
#include "table.h"
#include "set.h"
#include "getword.h"
#include "mem.h"
#include "examples.h"

// xref prototypes 104

int compare(const void * x, const void * y) {
    return str_cmp(x, y);
}

//xref data 105

/**
 * 全局变量, 每次first遇到一个换行符时,都将linenumjia1
 */
int linenum;

//xref functions 102

int cmpint(const void *x, const void * y) {
    if(**(int **)x < **(int **)y) {
        return -1;
    } else if(**(int **)x > **(int **)y) {
        return 1;
    } else {
        return 0;
    }
}

int intcmp(const void *x, const void * y) {
    return cmpint(&x, &y);
}

unsigned  int inthash(const void *x) {
    return *(int *)x;
}

/**
 * first是传递给getword的函数指针参数
 * 用于识别标识符的首字母
 * @param c
 * @return
 */
int first(int c) {
    if(c == '\n'){
        linenum++;
    }
    return isalpha(c) || c == '_';
}

int rest(int c) {
    return isalpha(c) || c == '_' || isdigit(c);
}

void print_table(Table_T files) {
    int i;
    void **array = Table_toArray(files, NULL);
    qsort(array, Table_length(files), 2* sizeof(*compare), compare);

    for (i = 0;  array[i] ; i+=2) {
        if(*(char *)array[i] != '\0') {
            printf("\t%s:", (char *)array[i]);
        }

        //print the line numbers in the set array[i+1] 104
        int j;
        void **lines = Set_toArray(array[i+1], NULL);
        qsort(lines, Set_length(array[i+1]), sizeof(*lines), cmpint);
        for(j = 0; lines[j]; j++) {
            printf(" %d", *(int *)lines[j]);
        }
        FREE(lines);

        printf("\n");
    }
}

void apply3(const void *key, void *cl) {
    printf("%d ", *(int *)key);
}

void apply2(const void *key, void ** value, void * cl) {
    printf("\n\tItem key is %s\n", (char *)key);
    printf("\tItem value is:[");
    Set_T * subItem = (Set_T *)value;
    Set_map(*subItem, apply3, NULL);
    printf("]\n");
}

void apply1(const void *key, void ** value, void * cl){
    printf("KEY is %s\n", (char *)key);
    printf("VALUE is [");
    Table_T * item = (Table_T *)value;
    Table_map(*item, apply2, NULL);
    printf("\t]\n");
}

/**
 * 交叉引用表的创建
 * @param name   文件路径
 * @param fp     文件句柄
 * @param identifiers Table_T *类型
 *              文件名作为key, 文件中的标识符和其出现的所有行号的set集合主城的key-value作为value
 *              这是一个Table_T 嵌套Table_T的结构, 而且还有一个set集合作为value
 */
void xref(/*in*/const char * name, /*in*/FILE *fp, /* in out */Table_T * identifiers) {
    char buf[128];
    if(name == NULL) {
        name = "";
    }
    name = Atom_string(name);
    linenum = 1;

    while(getword(fp , buf, sizeof(buf), first, rest)) {
        Set_T set;
        Table_T files;
        const char * id = Atom_string(buf);

        //files <- file table in identifiers associated with id 106
        //在第一次遇到某个标识符时,identifiers表中没有对应的项,因此需要创建文件表
        //并将 "标识符-文件表" 对 添加到identifiers表中
        files = Table_get(*identifiers, id);
        if(files == NULL) {
            files = Table_new(NULL, NULL);
            Table_put(identifiers, id, files);
        }

        //set <- set in files associated with anme 106
        //在一个新文件中每一次遇到某个标识符时, 行号的集合尚不存在,因此需要创建一个新集合并将其添加到files表中
        set = Table_get(files, name);
        if(set == NULL) {
            set = Set_new(0, intcmp, inthash);
            Table_put(&files, name, set);
        }

        //add linenum to set, if necessary 107
        // 这里set就是应该插入当前行号的目标集合
        int * p = &linenum;  //得到行号
        if(!Set_member(set, p)) {  //判断集合中有没有这个行号
            NEW(p);
            *p = linenum;
            Set_put(set, p);  //将行号加入到集合中
        }
    }
}

int test_xref(int argc, char * argv[]) {
    int i;

    Table_T table = Table_new(NULL, NULL);
    Table_T * identifiers = &table;

    for (i = 1; i < argc; i++) {
        FILE *fp = fopen(argv[i], "r");  //打开文件
        if (fp == NULL) {  //打开失败
            fprintf(stderr, "%s: can't oepn '%s' (%s)\n", argv[0], argv[i], strerror(errno));
            return EXIT_FAILURE;
        } else {
            xref(argv[i], fp, identifiers);
            fclose(fp);
        }
    }

//    if(argc == 1) {
//        xref(NULL, stdin, identifiers);
//    }

    if (Table_length(*identifiers) > 0) {
        //print the identifiers 103

        printf("identifiers's length is %d.\n", Table_length(*identifiers));
        Table_map(*identifiers, apply1, NULL);

        void **array = Table_toArray(*identifiers, NULL);
        //排序
        qsort(array, Table_length(*identifiers), 2 * sizeof(*array), compare);

        for (i = 0; array[i]; i += 2) {
            printf("%s", (char *) array[i]);
            print_table(array[i + 1]);
        }
    } else {
        printf("empty xref table.\n");
    }
    return EXIT_SUCCESS;
}