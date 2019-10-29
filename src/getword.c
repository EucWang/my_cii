//
// Created by wangxn on 2019/10/29.
//

#include "getword.h"
#include "assert.h"

#include <ctype.h>
#include <string.h>
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
int getword(/*in*/FILE * fp,
            /*out*/char * buf,
            /*in*/int size,
            /*in*/int first(int c),
            /*in*/int rest(int c)) {

    int i = 0;
    int c;

    assert(fp && buf && size > 1 && first && rest);

    c = getc(fp);

    for (; c != EOF; c =getc(fp)) {
        if (first(c)) {
            //-=store c in buf if it fits 88--
            if (i < size -1) {
                buf[i++] = c;
            }
            c = getc(fp);
            break;
        }
    }

    for (; c != EOF && rest(c); c = getc(fp)) {
        //--store c in buf if it fits 88--
        if (i < size -1) {
            buf[i++] = c;
        }
    }

    if(i < size) {
        buf[i] = '\0';
    } else {
        buf[size-1] = '\0';
    }

    if (c != EOF) {
        ungetc(c, fp);
    }

    return i>0;
}
