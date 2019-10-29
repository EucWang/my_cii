//
// Created by wangxn on 2019/10/29.
//

#ifndef ICD_GETWORD_H
#define ICD_GETWORD_H

#include <stdio.h>

/**
 * 从打开的文件fp中读取下一个单词,将其作为0结尾的字符串存入到buf中,并返回1.
 * 当到达文件尾而无法读取单词时,将返回0
 * @param fp  FILE
 * @param buf    不能为NULL
 * @param size   必须大于1
 * @param first  不能为NULL
 * @param rest  不能为NULL
 * @return 返回1,表示获取成功,0表示获取失败
 */
extern int getword(/*in*/FILE * fp, /*out*/char * buf, int size, int first(int c), int rest(int c));

#endif //ICD_GETWORD_H
