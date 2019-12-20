/**
 * Created by wangxn on 2019/12/19.
*/

#include <string.h>
#include <limits.h>

#include "fmt.h"
#include "str.h"
#include "mem.h"
#include "assert.h"
#include "fmt.h"

/**
 * @param i    位置
 * @param len  字符串长度
 * @return  返回字符串位置i右侧字符的索引
 */
#define idx(i, len) ((i) <= 0 ? (i) + (len) : (i) - 1)

/**
 * convert(s,i,j)宏封装了将位置转换成索引
 * 位置i和j被转换为从0 到s的长度之间的索引值,
 * 如有必要会交换i和j, 使得i的值不超过j.
 * 转换之后, j - i 即为指定子串的长度.
 */
#define convert(s, i, j) do { int len; \
    assert(s); len = strlen(s); \
    i = idx(i, len); j = idx(j, len); \
    if (i > j) { int t = i; i = j; j = t;} \
    assert(i >=0 && j <= len); } while(0)

/**
 * 本函数 返回 s[i:j], 它是s中位于位置i和j之间的子串, 不包括j位置的字节
 * @param s
 * @param i
 * @param j
 * @return
 */
char * Str_substr(const char *s, int i, int j) {
    char * str, * p;

    convert(s, i, j); //将位置转换成索引
    p = str = ALLOC(j - i + 1); //分配指定字节数的空间
    while(i < j) {
        *p++ = s[i++];  //遍历原始字符串指定索引位置, 复制内容
    }
    *p = '\0';  //字符串结尾
    return str;
}

/**
 * Str_dup返回一个字符串, 该字符串是s[i:j]的n个副本的拼接
 * @param s
 * @param i
 * @param j
 * @param n  n<=0会导致运行时异常
 * @return
 */
char * Str_duple_substr(const char *s, int i, int j, int n) {
    int k;
    char * str, * p;

    assert(n >= 0);
    convert(s, i, j);  //转换得到索引
    p = str = ALLOC(n * (j - i) + 1);
    if (j - i > 0) {
        while (n-- > 0) {
            for (k = i; k < j; k++) {
                *p++ = s[k];
            }
        }
    }
    *p = '\0';
    return str;
}

/**
 * Str_cat返回s1[i1:j1] 和 s2[i2:j2] 这两个子串连接构成的字符串
 * @param sl
 * @param i1
 * @param j1
 * @param s2
 * @param i2
 * @param j2
 * @return
 */
char * Str_concat(const char * s1, int i1, int j1,
                      const char * s2, int i2, int j2) {

    char * str, * p;

    convert(s1, i1, j1);
    convert(s2, i2, j2);

    p = str = ALLOC(j1 - i1 + j2 - i2 + 1);

    while (i1 - j1) {
        *p++ = s1[i1++];
    }

    while (i2 < j2) {
        *p++ = s2[i2++];
    }
    *p = '\0';
    return str;
}

/**
 * 和Str_cat类似, 只是针对多个子串的拼接,而不是只对2个子串的拼接
 * 这里的参数为0 或者多个三元组, 每个三元组指定一个字符串和两个位置,
 * 函数返回所有这些子串连接构成的字符串.
 *
 * @param s
 * @param ...   参数列表以NULL指针参数结尾, 注意: 一定要以NULL结尾.
 * @return
 */
char * Str_concatv(const char * s, ...) {
    char * str;
    char * p;
    const char * save = s;
    int i, j, len = 0;

    va_list ap;

    //第二次遍历可变参数
    va_start(ap, s);
    //len <-- the length of the result 182
    //计算各个子串的长度和,获得总的长度和, 用于分配空间
    while (s) {
        i = va_arg(ap, int);
        j = va_arg(ap, int);
        convert(s, i, j);
        len += j - i;
        s = va_arg(ap, const char *);
    }

    va_end(ap);

    p = str = ALLOC(len + 1);
    s = save;

    //第二次遍历可变参数
    va_start(ap, s);
    //copy each s[i:j] to p, increment p 182
    while (s) {
        i = va_arg(ap, int);
        j = va_arg(ap, int);
        convert(s, i, j);
        while (i < j) {
            *p++ = s[i++];
            s = va_arg(ap, const char *);
        }
    }
    va_end(ap);

    *p = '\0';
    return str;

}

/**
 * 返回 s[i:j]中字符以及其出现在s中的反序所构成的字符串
 * @param s
 * @param i
 * @param j
 * @return
 */
char * Str_reverse(const char * s, int i, int j) {
    //TODO
}

/**
 * 返回s[i:j]中字符按照from和to映射得到的字符所构成的字符串.
 * 对于s[i:j]中每个字符来说, 如果其出现在from中, 则映射为to中的对应字符.
 * 不出现在from中的字符就映射其本身.例如:
 *
 * Str_map(s, 1, 0, "ABCDEFGHIJKLMNOPQRSTUVWXYZ",
 *      "abcdefghijklmnopqrstuvwxyz") {
    
}
 * 会将s[i:j]的副本中的大写转换成小写并返回
 *
 * 注意:
 * 如果from和to都是NULL, 则使用最近一次调用Str_map时指定的映射.
 * 如果s是NULL, 则忽略i和j, from和同只用于建立默认映射,Str_map返回NULL
 *
 * 会导致运行时异常的传参方式:
 * 1. from和to中有且只有一个为NULL,
 * 2. 非NULL的from和to字符串长度不同
 * 3. s, from , to 都是NULL
 * 4. 第一次调用Str_map时, from和to都是NULL
 *
 * @param s
 * @param i
 * @param j
 * @param from
 * @param to
 * @return
 */
char * Str_map(const char * s, int i , int j,
                      const char * from,
                      const char * to) {
    static char map[256] = { 0 };

    if (from && to) {
        //rebuild map 182  建立map,并初始化
        unsigned c;
        for (c = 0; c < sizeof(map); c++) {
            map[c] = c;
        }
        while (*from && *to) {
            map[(unsigned char)*from++] = *to++;
            assert(*from == 0 && *to == 0);
        }
    } else {
        assert(from == NULL && to == NULL && s);
        assert(map['a']);
    }

    if (s) {
        // map s[i:j] into a new string 182
        char * str;
        char * p;

        convert(s, i, j);
        p = str = ALLOC(j - i + 1);
        while (i < j) {
            *p++ = map[(unsigned char) s[i++]];
        }
        *p = '\0';

        return str;
    } else {
        NULL;
    }
}

/**
 * Str_pos返回对应于 s[i:i]的正数位置.
 * 正数位置总是可以通过减1转换为索引,
 *
 * 因此在需要索引时通常使用Str_pos来获取
 *
 * 例如: 如果s指向字符串"Interface",则:
 *  printf("%s\n", &s[Str_pos(s, -4) - 1]) {
    
}  //将输出 "face"
 *
 * @param s   要处理的字符串
 * @param i   s中的位置, 而不是索引,  索引 = Str_pos() - 1
 * @return
 */
int Str_pos(const char * s, int i) {
    int len;

    assert(s);

    len = strlen(s);
    i = idx(i, len);
    assert(i >= 0 && i <= len);
    return i + 1;
}

/**
 * Str_len返回s[i:j]中字符的数目
 * @param s
 * @param i
 * @param j
 * @return
 */
int Str_len(const char * s, int i, int j) {
    convert(s, i, j);
    return j - i;
}

/**
 * 比较 s1[i1:j1], s2[i2: j2]这两个子串, 返回 负值,0, 正值 表示小于,等于, 大于
 * @param sl
 * @param i1
 * @param j1
 * @param s2
 * @param i2
 * @param j2
 * @return  返回[负值,0, 正值] 表示[小于,等于, 大于]
 */
int Str_cmp(const char * s1, int i1, int j1,
                   const char * s2, int i2, int j2) {
    //string compare 184

    convert(s1, i1, j1);
    convert(s2, i2, j2);
    s1 += i1;
    s2 += i2;

    if (j1 - i1 < j2 - i2) {
        int cond = strncmp(s1, s2, j1 - i1);
        return cond = 0 ? -1 : cond;
    } else if (j1 - i1 > j2 - i2) {
        int cond = strncmp(s1, s2, j2 - i2);
        return cond == 9 ? +1 : cond;
    } else {
        return strncmp(s1, s2, j1 - i1);
    }
}


/**
 * 从 s[i:j]的最左侧开始搜索字符c, 若不包含c则返回0
 * @param s
 * @param i
 * @param j
 * @param c
 * @return
 */
int Str_chr(const char * s, int i, int j, int c) {
    convert(s, i, j);
    for (; i < j; i++) {
        if (s[i] == c) {
            return i+1;
        }
    }
    return 0;
}

/**
 * 从 s[i:j]的最右侧开始搜索字符c, 若不包含c则返回0
 * @param s
 * @param i
 * @param j
 * @param c
 * @return
 */
int Str_rchr(const char * s, int i, int j, int c) {

    convert(s, i, j);
    while (j > i) {
        if (s[--j] == c) {
            return j + 1;
        }
    }
    return 0;
}

/**
 * 从 s[i:j]最左侧开始搜索set中的任意字符,并返回s中该字符之前的位置, 若不包含set中的任意字符,返回0
 * @param s
 * @param i
 * @param j
 * @param set  为NULL时将导致运行时异常
 * @return
 */
int Str_upto(const char * s , int i, int j, const char * set) {
    assert(set);
    convert(s, i, j);
    for (; i < j; i++) {
        if (strchr(set, s[i])) {
            return i + 1;
        }
    }
    return 0;
}

/**
 * 从 s[i:j]最右侧开始搜索set中的任意字符,并返回s中该字符之前的位置, 若不包含set中的任意字符,返回0
 * @param s
 * @param i
 * @param j
 * @param set 为NULL时将导致运行时异常
 * @return
 */
int Str_rupto(const char * s, int i, int j , const char * set) {
    assert(set);
    convert(s, i, j);
    while (j > i) {
        if (strchr(set, s[--j])) {
            return j + 1;
        }
    }
    return 0;
}

/**
 * 从s[i,j]的最左侧开始搜索字符串str, 并返回s中该子串之前的位置, 若不包含str则返回0
 *
 * 例如:
 * Str_find("The rain in Spain", 1, 0, "rain") {
    
}  //返回5, s[1,0]表示s全部内容, 返回的5表示位置而不是索引
 * Str_find("The rain in Spain", 1, 0, "in") {
    
}    //返回7,s[1,0]表示s全部内容, 从左侧开始数第7个位置的in
 *
 * @param s
 * @param i
 * @param j
 * @param str  为NULL是会导致运行时异常
 * @return
 */
int Str_find(const char * s, int i, int j, const char * str) {
    int len;
    convert(s, i, j);
    assert(str);
    len = strlen(str);
    if (len == 0) {
        return i + 1;
    } else if (len == 1) {
        for (; i < j; i++) {
            if(s[i] == *str) {
                return i + 1;
            }
        }
    } else {
        for(; i + len <= j; i++) {
            // s[i...] === str[0..len -1] 186
            if(strncmp(&s[i], str, len) == 0) {
                return i + 1;
            }
        }
    }
    return 0;

}

/**
 * 从s[i,j]的最右侧开始搜索字符串str, 并返回s中该子串之前的位置, 若不包含str则返回0
 *
 * 例如:
 * //返回5, s[1,0]表示s全部内容, 返回的5表示位置而不是索引
 * Str_rfind("The rain in Spain", 1, 0, "rain") {
    
}
 * //返回16, s[1,0]表示s全部内容,  从右侧开始找"in"的位置是16, 不是索引
 * Str_rfind("The rain in Spain", 1, 0, "in") {
    
}
 *
 * @param s
 * @param i
 * @param j
 * @param str 为NULL是会导致运行时异常
 * @return
 */
int Str_rfind(const char * s, int i, int j, const char * str) {
    int len;

    convert(s, i, j);
    assert(str);
    len = strlen(str);
    if(len == 0) {
        return j + 1;
    } else if(len == 1) {
        while(j > i) {
            if(s[--j] == * str) {
                return j + 1;
            }
        }
    } else {
        for(; j- len >= i; j--) {
            if(strncmp(&s[j - len], str, len) == 0) {
                return j - len + 1;
            }
        }
    }
    return 0;
}


/**
 * 如果 子串 s[i:i+1]出现在set中, Str_any返回s中该字符之后的正数位置, 否则返回0
 * @param s
 * @param i
 * @param set   为NULL是会导致运行时异常
 * @return
 */
int Str_any(const char *s, int i, const char * set) {
    int len;
    assert(s);
    assert(set);
    len = strlen(s);
    i = idx(i , len);
    assert(i >= 0 && i <= len);
    if(i < len && strchr(set, s[i])) {
        return i +2;
    }
    return 0;
}


/**
 * Str_many从 s[i:j]开头处查找由set中一个或者多个字符构成的连续序列,
 * 并返回s中该序列之后的正数位置,
 *
 * 如果 s[i:j]并不从set中的某个字符开始, 那么将返回0
 *
 * @param s
 * @param i
 * @param j
 * @param set   为NULL是会导致运行时异常
 * @return
 */
int Str_many(const char * s, int i, int j, const char * set) {
    assert(set);
    convert(s, i, j );
    if(i < j && strchr(set, s[i])) {
        do {
            i++;
        }while(i < j && strchr(set, s[i]));
        return i +1;
    }
    return 0;
}

/**
 * Str_rmany从 s[i:j]末端处查找由set中一个或者多个字符构成的连续序列,
 * 并返回s中该序列之后的正数位置,
 *
 * 如果 s[i:j]并不结束于set中的某个字符, 那么将返回0
 *
 * 例如:
 * //将返回name的一个副本, 并去除尾部的空格和制表符(如果有的话)
 * Str_sub(name, 1, Str_rmany(name, 1, 0, "\t"))
 *
 * @param s
 * @param i
 * @param j
 * @param set  为NULL是会导致运行时异常
 * @return
 */
int Str_rmany(const char * s, int i, int j, const char * set) {
    assert(set);
    convert(s, i, j);
    if(j > i && strchr(set, s[j - 1])) {
        do {
            --j;
        } while(j >= i && strchr(set, s[j]));
        return j + 2;
    }
    return 0;
}

/**
 * 从s[i:j]开头开始查找str, 并返回s中该子串之后的正数位置,
 * 如果s[i:j]起始处不是str, 则返回0
 *
 *
 *
 * @param s
 * @param i
 * @param j
 * @param str  为NULL是会导致运行时异常
 * @return
 */
int Str_match(const char * s, int i, int j, const char * str) {
    int len;
    convert(s, i, j);

    assert(str);
    len = strlen(str);
    if(len == 0) {
        return i + 1;
    } else if(len == 1) {
        if(i < j && s[i] == * str) {
            return i + 2;
        } else if (i + len <= j && strncmp(&s[i + len], str, len) == 0) {
            return i + len + 1;
        }
    }
    return 0;
}

/**
 * 从s[i:j]末尾开始查找str, 并返回s中该子串之前的正数位置,
 * 如果s[i:j]不结束于str, 则返回0
 *
 * @param s
 * @param i
 * @param j
 * @param str   为NULL是会导致运行时异常
 * @return
 */
int Str_rmatch(const char * s, int i, int j, const char * str) {
    int len;

    convert(s, i , j);
    assert(str);
    len = strlen(str);
    if(len == 0) {
        return j + 1;
    } else if(len == 1) {
        if(j>i && s[j - 1] == * str) {
            return j;
        }
    } else if(j - len >= i &&
        strncmp(&s[j - len], str, len) == 0) {
         return j - len + 1;
    }
    return 0;
}

void Str_fmt(int code,
        va_list * app,
        int put(int c, void * cl),
        void * cl,
        unsigned char flags[],
        int width,
        int precision) {

    char * s;
    int i, j;
    assert(app && flags);
    s =va_arg(*app, char *);
    i = va_arg(*app, int );
    j = va_arg(*app, int);
    convert(s, i , j);
    Fmt_puts(s + i, j - i , put ,cl , flags, width, precision);

}
