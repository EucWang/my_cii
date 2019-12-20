/**
 * Created by wangxn on 2019/12/19.
 *
 *string.h中定义的大部分函数都有两个缺点:
 * 1. 客户程序必须为结果分配空间
 * 2. 这些函数都是不安全的, 其中任何一个函数都无法检查结果字符串是否包含足够的空间
 *
 * 本接口中定义的函数可以避免这些缺点,
 * 并提供一个方便的方法来操作其字符串参数的子串.
 * 并且这些函数比string.h中的更加安全.
 * 因为大部分Str函数都会为其结果分配空间.
 *
 * 类似于string.h函数, Str函数的客户程序任然必须释放返回的结果.
 * Text接口, 可以避免Str函数的一些分配开销.
 *
 * Str接口中的函数的所有字符串参数,都通过一个指向0结尾字符数组的指针和该数组中的位置给出.
 * 类似于Ring中的位置
 * 字符串位置标识了各个字符之间的位置, 以及最后一个非0字符之后的位置.
 *
 * 如下, 给出了一个 "Interface" 中的各个位置:
 *  1   2   3   4   5   6   7   8   9   10     <-- 这是正位置,pos, 取值 1 ~ (n+1)
 *  |   |   |   |   |   |   |   |   |   |
 *  V   V   V   V   V   V   V   V   V   V
 *  ------------------------------------
 *  | I | n | t | e | r | f | a | c | e   <-- 这是索引值 index
 *  ------------------------------------
 *  ^   ^   ^   ^   ^   ^   ^   ^   ^   ^
 *  |   |   |   |   |   |   |   |   |   |
 * -9  -8  -7  -6  -5  -4  -3  -2  -1   0     <-- 这是负位置, pos, 取值 -n ~ 0
 *
 *
 * 字符串s中的两个位置i和j指定了两个位置之间的子串,记做s[i:j]
 * 如果s指向字符串Interface,
 * 那么s[-4:0] 同样指定了子串 face
 * 子串可以为 NULL,
 * s[3:3] 和 s[3:-7] 都指定了Interface中n和t之间的NULL子串.
 * 对任何有效位置i, s[i: i+1] 总是i右侧的字符(最右侧的位置除外)
 *
 * 这里不支持字符索引, 因为位置比字符索引效果更好, 这也是python采用的方式, 因为避免了令人迷惑的边界情况.
 * 而且非正数位置可以在不知道字符串长度的情况下访问字符串
 *
 * Str接口函数可以创建并返回0结尾的字符串,且返回有关其中字符串和位置的信息.
 *
 *
 * 一个示例:
 * //从路径中截取文件名称, 不包含suffix后缀
 * char * basename(char * path, int i, int j, const char * suffix) {
 *     i = Str_rchr(path, i, j, '/');         //从右侧开始找"/"的位置
 *     j = Str_rmatch(path, i+1, 0, suffix);  //从右侧开始匹配 suffix (文件后缀的位置)
 *     return Str_dup(path, i + 1, j, 1);     //Str_dup返回一个字符串, 该字符串是s[i:j]的n个副本的拼接,
 * }
 *
 * //调用示例:
 * basename("/usr/jenny/main.c", 1, 0, ".c")       //返回main
 * basename("../src/main.c", 1, 0, "")             //返回main.c
 * basename("main.c", 1, 0, "c")                   //返回main
 * basename("main.c", 1, 0, ".obj") main. c        //返回main.c
 * basename("examples/wfmain.c", 1, 0, ".main.c")  //wf
 * 使用 Str_ rchr 来 查找 最 右侧 的 斜线， 使用 Str_ rmatch 来 定位 后缀。
 */

#ifndef ICD_STR_H
#define ICD_STR_H

#include <stdarg.h>

//exported functions 174

/**
 * 本函数 返回 s[i:j], 它是s中位于位置i和j之间的子串
 * @param s
 * @param i
 * @param j
 * @return
 */
extern char * Str_substr(const char *s, int i, int j);

/**
 * Str_dup返回一个字符串, 该字符串是s[i:j]的n个副本的拼接
 * @param s
 * @param i
 * @param j
 * @param n  n<=0会导致运行时异常
 * @return
 */
extern char * Str_duple_substr(const char *s, int i, int j, int n);

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
extern char * Str_cat(const char * sl, int i1, int j1,
        const char * s2, int i2, int j2);

/**
 * 和Str_cat类似, 只是针对多个子串的拼接,而不是只对2个子串的拼接
 * 这里的参数为0 或者多个三元组, 每个三元组指定一个字符串和两个位置,
 * 函数返回所有这些子串连接构成的字符串.
 *
 * @param s
 * @param ...   参数列表以NULL指针参数结尾, 注意: 一定要以NULL结尾.
 * @return
 */
extern char * Str_catv(const cahr * s, ...);

/**
 * 返回 s[i:j]中字符以及其出现在s中的反序所构成的字符串
 * @param s
 * @param i
 * @param j
 * @return
 */
extern char * Str_reverse(const char * s, int i, int j);

/**
 * 返回s[i:j]中字符按照from和to映射得到的字符所构成的字符串.
 * 对于s[i:j]中每个字符来说, 如果其出现在from中, 则映射为to中的对应字符.
 * 不出现在from中的字符就映射其本身.例如:
 *
 * Str_map(s, 1, 0, "ABCDEFGHIJKLMNOPQRSTUVWXYZ",
 *      "abcdefghijklmnopqrstuvwxyz");
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
extern char * Str_map(const char * s, int i , int j,
        const char * from,
        const char * to);

/**
 * Str_pos返回对应于 s[i:i]的正数位置.
 * 正数位置总是可以通过减1转换为索引,
 *
 * 因此在需要索引时通常使用Str_pos来获取
 *
 * 例如: 如果s指向字符串"Interface",则:
 *  printf("%s\n", &s[Str_pos(s, -4) - 1]);  //将输出 "face"
 *
 * @param s   要处理的字符串
 * @param i   s中的位置, 而不是索引,  索引 = Str_pos() - 1
 * @return
 */
extern int Str_pos(const char * s, int i);

/**
 * Str_len返回s[i:j]中字符的数目
 * @param s
 * @param i
 * @param j
 * @return
 */
extern int Str_len(const char * s, int i, int j);

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
extern int Str_cmp(const char * sl, int i1, int j1,
        const char * s2, int i2, int j2);


/**
 * 从 s[i:j]的最左侧开始搜索字符c, 若不包含c则返回0
 * @param s
 * @param i
 * @param j
 * @param c
 * @return
 */
extern int Str_chr(const char * s, int i, int j, int c);

/**
 * 从 s[i:j]的最右侧开始搜索字符c, 若不包含c则返回0
 * @param s
 * @param i
 * @param j
 * @param c
 * @return
 */
extern int Str_rchr(const char * s, int i, int j, int c);

/**
 * 从 s[i:j]最左侧开始搜索set中的任意字符,并返回s中该字符之前的位置, 若不包含set中的任意字符,返回0
 * @param s
 * @param i
 * @param j
 * @param set  为NULL时将导致运行时异常
 * @return
 */
extern int Str_upto(const char * s , int i, int j, const char * set);

/**
 * 从 s[i:j]最右侧开始搜索set中的任意字符,并返回s中该字符之前的位置, 若不包含set中的任意字符,返回0
 * @param s
 * @param i
 * @param j
 * @param set 为NULL时将导致运行时异常
 * @return
 */
extern int Str_rupto(const char * s, int i, int j , const char * set);

/**
 * 从s[i,j]的最左侧开始搜索字符串str, 并返回s中该子串之前的位置, 若不包含str则返回0
 *
 * 例如:
 * Str_find("The rain in Spain", 1, 0, "rain");  //返回5, s[1,0]表示s全部内容, 返回的5表示位置而不是索引
 * Str_find("The rain in Spain", 1, 0, "in");    //返回7,s[1,0]表示s全部内容, 从左侧开始数第7个位置的in
 *
 * @param s
 * @param i
 * @param j
 * @param str  为NULL是会导致运行时异常
 * @return
 */
extern int Str_find(const char * s, int i, int j, const char * str);

/**
 * 从s[i,j]的最右侧开始搜索字符串str, 并返回s中该子串之前的位置, 若不包含str则返回0
 *
 * 例如:
 * //返回5, s[1,0]表示s全部内容, 返回的5表示位置而不是索引
 * Str_rfind("The rain in Spain", 1, 0, "rain");
 * //返回16, s[1,0]表示s全部内容,  从右侧开始找"in"的位置是16, 不是索引
 * Str_rfind("The rain in Spain", 1, 0, "in");
 *
 * @param s
 * @param i
 * @param j
 * @param str 为NULL是会导致运行时异常
 * @return
 */
extern int Str_rfind(const char * s, int i, int j, const char * str);


/**
 * 如果 子串 s[i:i+1]出现在set中, Str_any返回s中该字符之后的正数位置, 否则返回0
 * @param s
 * @param i
 * @param set   为NULL是会导致运行时异常
 * @return
 */
extern int Str_any(const char *s, int i, const char * set);

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
extern int Str_many(const char * s, int i, int j, const char * set);

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
extern int Str_rmany(const char * s, int i, int j, const char * set);

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
extern int Str_match(const char * s, int i, int j, const char * str);

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
extern int Str_rmatch(const char * s, int i, int j, const char * str);


#endif //ICD_STR_H
