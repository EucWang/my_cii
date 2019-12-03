/**
 * Created by wangxn on 2019/12/3.
*/

#include <stdarg.h>
#include <string.h>
#include "bit.h"
#include "assert.h"
#include "mem.h"

#define T Bit_T

struct T {
    int length;             // 向量中比特位的数目
    unsigned char * bytes;  // 指向一个至少包含 <length/8>个字节的内存区
    unsigned long * words;  //
};

#define BPW (8 * sizeof(unsigned long))

#define nwords(len) ((((len) + BPW - 1) & (~(BPW - 1))) / BPW)

/**
 * 宏nbytes(len) 计算了 len/8,
 */
#define nbytes(len) ((((len) + 8 -1)&(~(8-1)))/8)

/**
 * 高位的掩码表
 * index == 0, 没有0位
 * index == 1, 只有最低位是0
 * index == 2, 最低2位是0
 * ...
 * index == 7, 只有最高位是1, 其他位全部是0
 */
unsigned  char msbmask[] = {
        //1111 1111,    1111 1110,  1111 1100,  1111 1000
        0xff, 0xfe, 0xfc, 0xf8,
        //1111 0000,    1110 0000,  1100 0000,  1000 0000
        0xf0, 0xe0, 0xc0, 0x80
};
/**
 * 低位的掩码表2
 * index == 0, 最低位1
 * index == 1, 最低位两位为1
 * index == 2, 最低3位为1
 * ...
 * index == 7, 全部为1
 */
unsigned char lsbmask[] = {
        //0000 0001,    0000 0011,  0000 0111,  0000 1111
        0x01, 0x03, 0x07,0x0f,
        //0001 1111,    0011 1111,  0111 1111,  1111 1111
        0x1f, 0x3f, 0x7f, 0xff
};

/**
 * 这个数组的值与其索引一一对应,对应到一个字节中有多少个1
 * 比如index=15时, 15的二进制是1111, 即有4个1,则 count[15] = 4
 * 比如index= 10时, 10的二进制是1010, 即有2个1, 则 count[10] = 2
 */
static char count[] = {
        0,1,1,2,
        1,2,2,3,
        1,2,2,3,
        2,3,3,4
};

/**
 * Bit_new创建一个包含length个比特位的新向量,并将所有比特位都设置为0.
 * 该向量表示了从0到length-1 的所有整数(包含0 和 length-1)
 * @param length 不能为负数,为负数则导致运行时异常
 * @return Bit_T
 */
extern T Bit_new(int length) {
    T set;
    assert(length >= 0);
    NEW(set);
    if(length > 0) {
        //最多可能分配sizeof(unsign long) -1 个多余的字节
        set->words = CALLOC(nwords(length), sizeof(unsigned long));
    } else {
        set->words = NULL;
    }
    set->bytes = (unsigned char *)set->words;
    set->length = length;
    return set;
}

/**
 * 返回Bit_T中的比特位数.
 * @param set   不能为NULL, 为NULL会导致运行时异常
 * @return
 */
extern int Bit_length(T set) {
    assert(set);
    return set->length;
}

/**
 * 返回Bit_T中1个数目, (即置位的比特位数)
 * @param set 不能为NULL, 为NULL会导致运行时异常
 * @return
 */
extern int Bit_count(T set) {
    assert(set);

    int length = 0;
    int n;

    for(n = nbytes(set->length); --n >= 0; ) {
        unsigned char c = set->bytes[n];        //得到Bit_T中的一个字节
        length += count[c & 0xF] + count[c>>4]; //分别计算高4位和低4位中的1的个数,然后相加得到一个字节中的1的个数
    }
    return length;
}

/**
 * 释放 *set 并将*set置为NULL
 * @param set   不能为NULL, 为NULL会导致运行时异常
 */
extern void Bit_free(T * set) {
    assert(set && * set);
    FREE((*set)->words);
    FREE(*set);
}

/**
 * 返回比特位n,
 *
 * @param set  不能为NULL,,否则会导致运行时错误
 * @param n   不能为负值, 不能大于等于set的长度,否则会导致运行时错误
 * @return
 */
extern int Bit_get(T set, int n) {
    assert(set);
    assert(n >= 0 && set->length > n);

    //bit n in set 147
    unsigned  char c = set->bytes[n/8];  //先计算得到n位所在的字节
    return c >> (n % 8) & 1;             //然后将这个字节中的n位右移到最低位,然后和1按位与操作
}

/**
 * 将比特位n设置为bit,并返回该比特位的原值.
 * @param set  不能为NULL,,否则会导致运行时错误
 * @param n    不能为负值或者是大于等于set的长度,否则导致运行时异常
 * @param bit  不能是0和1以外的值,否则导致运行时错误
 * @return
 */
extern int Bit_put(T set, int n, int bit) {
    assert(set);
    assert(bit == 0 || bit == 1);
    assert(n >= 0 && set->length > n);

    int prev = Bit_get(set, n);

    if (bit == 1) {
        set->bytes[n/8] |= 1<<(n%8);   //(0000 0100)按位或之后和原值相加
    } else {
        set->bytes[n/8] &= ~(1<<(n%8));  // 取反之后(1111 1011)和原值按位与的结果,再和原值相加
    }

    return prev;
}

/**
 * Bit_clear将low到high的所有比特位清零, 包含low和high比特位
 * @param set   不能为NULL,,否则会导致运行时错误
 * @param low   low不能大于high, 不能为负值,low/high 不能大于等于set的长度,否则会导致运行时错误
 * @param high  不能为负值,low/high 不能大于等于set的长度,否则会导致运行时错误
 */
extern void Bit_clear(T set, int low, int high) {
    //这里会将msbmask和lsbmask取反得到相应的掩码,分别按位与到low/8和high/8字节中即可
    assert(set);
    assert(low >= 0 && set->length > high);
    assert(high >= low);

    if(low/8 < high/8) {  //
        //当low和high指的是不同字节中的比特位时,
        int i;
        set->bytes[low/8] &= ~msbmask[low%8]; //低位所在字节部分清0

        for(i = low/8 + 1; i < high/8; i++) {   //中间字节全部清零
            set->bytes[i] = 0;
        }

        set->bytes[high/8] &= ~lsbmask[high%8];  //高位字节部分清零

    } else {
        //这里表示low位和high位在同一个字节中, 对掩码取反,即中间部分清零
        set->bytes[low/8] &= ~(msbmask[low%8] & lsbmask[high%8]);
    }
}

/**
 * Bit_set将low到high的所有比特位置 置为位1 , 含比特位low和high
 * @param set 不能为NULL,,否则会导致运行时错误
 * @param low
 * @param high
 */
extern void Bit_set(T set, int low, int high) {
    //check set, low, high 148
    assert(set);
    assert(low >= 0 && set->length > high);
    assert(high >= low);

    if(low/8 < high/8) {
        //当low和high指的是不同字节中的比特位时,
        //字节low/8中被置位的比特位数目取决于low%8
        //如果low%8==0, 该字节中所有比特位都被置位
        //如果low%8 == 7, 则只有最高位置位.
        // set the most significant bits in byte low/8 148
        set->bytes[low/8] |= msbmask[low%8];

        //set all the bits in bytes low/8+1 .. high/8-1 148
        //在这个区域中,每个字节中所有比特位都将被设置为1
        int i;
        for(i = low/8 + 1; i < high/8; i++) {
            set->bytes[i] = 0xff;
        }

        //set the least significant bits in byte high/8 148
        //high%8确定了字节high/8中那些比特位被置位
        //如果high%8 == 0, 仅最低位置位
        //如果high%8 == 7, 那么该字节中所有比特位均置位
        set->bytes[high/8] |= lsbmask[high%8];

    } else {
        //set bits low%8 .. high%8 in byte low/8 149
        //这里表示low位和high位在同一个字节中,
        //可以将msbmask[low%8] 和lsbmask[high%8]给出的掩码合并确定一个掩码,来确定需要置位的比特位
        set->bytes[low/8] |= (msbmask[low%8] & lsbmask[high%8]);
    }

}

/**
 * Bit_not将low到high所有比特位取反
 * @param set  不能为NULL,,否则会导致运行时错误
 * @param low  low不能大于high, 不能为负值,low/high 不能大于等于set的长度,否则会导致运行时错误
 * @param high 不能为负值,low/high 不能大于等于set的长度,否则会导致运行时错误
 */
extern void Bit_not(T set, int low ,int high) {
    //通过与适当掩码的按位异或来完成 取反
    //异或: 如果a,b不相同,则异或结果为1; 如果a,b相同,则异或结果为0
    assert(set);
    assert(low >= 0 && set->length > high);
    assert(high >= low);

    if(low/8 < high/8) {
        //当low和high指的是不同字节中的比特位时,

        int i;
        //这里低字节中的 部分通过和掩码进行异或操作获得取反的结果
        set->bytes[low/8] ^= msbmask[low%8]; //高位为1, 异或,原位为0,则异或结果为1; 原位为1, 则异或结果为0

        //中间字节中的全部位通过与0xff异或,获得取反的结果
        for (i = low/8 + 1; i < high/8 ; i++) {
            set->bytes[i] ^= 0xff;
        }

        //这里高字节中的部分 通过和掩码进行异或操作获得取反的结果
        set->bytes[high/8] ^= lsbmask[high%8];
    } else {
        //set bits low%8 .. high%8 in byte low/8 149
        //这里表示low位和high位在同一个字节中,
        //依然是通过异或操作获得取反的结果
        set->bytes[low/8] ^= (msbmask[low%8] & lsbmask[high%8]);
    }

}

/**
 *  如果s是t的一个真子集, 返回1, 否则返0
 * @param s 不能为NULL,,否则会导致运行时错误
 * @param t 不能为NULL,,否则会导致运行时错误, s和t的长度必须相同,否则会导致运行时错误
 * @return
 */
extern int Bit_lt(T s, T t) {
    assert(s && t);
    assert(s->length == t->length);
    int i;
    int lt = 0;
    for (i = nwords(s->length);  --i >= 0 ; ) {
        //如果s是t的真子集,并且s不等于t,可以通过检查两个条件来确认, 也就是这两个条件都必须满足才算
        if((s->words[i] & ~t->words[i]) != 0){
            //1.  对于每个i, 都有(s->u.words[i] & ~t->u.words[i]) == 0; (这也是Bit_leq()方法中的判断条件)
            //这个条件判断确保s是t的子集, 即s是t的真子集或者s等于t
            return 0;
        } else if(s->words[i] != t->words[i]) {
            //2.  至少有一个i, 使得 s->u.words[i] != t->u.words[i], 也就是s不等于t
            lt |= 1;
        }
        return lt;
    }

}

/**
 * 如果s == t, 则Bit_eq返回1, 否则返回0
 * @param s   如果s或者t为NULL, 或者s的长度和t的长度不同,则会导致运行时异常
 * @param t
 * @return  返回 1: 相同, 0 不相同
 */
extern int Bit_eq(T s, T t) {
    assert(s && t);
    assert(s->length == t->length);

    int i;

    for(i = nwords(s->length); --i>=0;) {
        if(s->words[i] != t->words[i]) {
            return 0;
        }
    }
    return 1;
}

/**
 * @param s  如果s或者t为NULL, 或者s的长度和t的长度不同,则会导致运行时异常
 * @param t
 * @return  如果s是t的一个真子集或者s等于t, 返回1, 否则返回0
 */
extern int Bit_leq(T s, T t) {
    assert(s && t);
    assert(s->length == t->length);

    int i;
    for (i = nwords(s->length);  --i >= 0; ) {
        //如果对于s中每个位置的比特位, t中对应的比特位都是1, 那么就有s是t的真子集
        // 也就是说s中为1的位,t中对应的位必为1; s中为0的位, t中可能为1可能为0;
        //从集合的角度看, 就是如果t的补集与s的交集为空, 那么s就是t的真子集.
        //就是说 s & ~t == 0,就是 s是t的真子集,
        //对于s和t中的每个unsigned long来说, 这个关系依然成立. 所有有下面的判断条件
        if(s->words[i] & ~t->words[i] != 0) {
            return 0;
        }
    }
    return 1;
}

/**
 * 从比特位0开始, 对set中的每一个比特位调用apply, n是比特位的编号
 * 介于0和集合的长度减1之间, bit是比特位n的值, cl有客户程序提供.
 * apply可以改变set本身
 * @param set
 * @param apply   apply不同于Table_map的函数, 这里可以改变set.若比特位n调用apply时,apply改变了比特位k,且k>n,则到k时修改可见
 * @param cl
 */
extern void Bit_map(T set, void apply(int n, int bit, void * cl), void * cl) {
    assert(set);
    assert(apply);

    int n;
    for(n = 0; n < set->length; n++) {
        apply(n, ((set->bytes[n/8])>>(n%8)) & 1, cl);
    }
}

#define setop(sequal, snull, tnull, op) \
    if(s == t) {assert(s); return sequal;} \
    else if(s == NULL){assert(t); return snull;} \
    else if(t == NULL){return tnull;} \
    else {\
    int i; T set; \
    assert(s->length == t->length); \
    set = Bit_new(s->length); \
    for(i = nwords(s->length); --i >= 0;) \
        set->words[i] = (s->words[i] op t->words[i]); \
    return set; }


static T copy(T t) {
    assert(t);
    T set;

    set = Bit_new(t->length);
    if(t->length > 0) {
        memcpy(set->bytes, t->bytes, nbytes(t->length));
    }
    return set;
}

/**
 * 返回s和t的并集 s + t, 实际上就是两个位向量的可兼容的按位或操作
 * @param s  s或t可以为NULL指针,但是不能同时为NULL; 若同时为NULL,或者长度不同,会导致运行时异常
 * @param t
 * @return  返回非NULL的Bit_T
 */
extern T Bit_union(T s, T t) {
    //实现集合操作的所有函数
    //可以按照每次一个长整数来处理集合,
    //因为这些功能与比特位编号无关
    //而且集合操作的函数还将NULL值解释为空集,
    //但是s和t中至少有一个不能为NULL值
    //这样才能确保结果集的长度.

    setop(copy(t), copy(t), copy(s), |)
}

/**
 * 返回s和t的交集 s * t, 就是两个位向量的按位与
 * @param s  s或t可以为NULL指针,但是不能同时为NULL; 若同时为NULL,或者长度不同,会导致运行时异常
 * @param t
 * @return  返回非NULL的Bit_T
 */
extern T Bit_inter(T s, T t) {
    //如果有任何一个参数是NULL, 则返回空集,
    //否则, 它将返回一个集合, 是其参数集合的按位与结果
    //TODO 这里可能会有问题
    setop(copy(t), Bit_new(t->length), Bit_new(s->length),&);
}

/**
 * 返回s和t的差集 (s - t), 是t的补集和s按位与
 * @param s  s或t可以为NULL指针,但是不能同时为NULL; 若同时为NULL,或者长度不同,会导致运行时异常
 * @param t
 * @return  返回非NULL的Bit_T
 */
extern T Bit_minus(T s, T t) {
    setop(Bit_new(s->length), Bit_new(t->length), copy(s), & ~);
}

/**
 * 返回s和t的对称差 s/t, 就是两个位向量的按位异或
 * @param s  s或t可以为NULL指针,但是不能同时为NULL; 若同时为NULL,或者长度不同,会导致运行时异常
 * @param t
 * @return  返回非NULL的Bit_T
 */
extern T Bit_diff(T s, T t) {
    //TODO 这里可能会有问题
    setop(Bit_new(s->length), copy(t), copy(s), &);
}
