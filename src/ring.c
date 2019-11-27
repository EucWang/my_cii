/**
 * Created by wangxn on 2019/11/27.
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zconf.h>
#include "assert.h"
#include "ring.h"
#include "mem.h"

#define T Ring_T

struct T {

    /**
     * head字段是指向Node结构的一个双链表,
     * Node结构中的的value字段保存环中的值
     *
     * head指向关联到索引0的值,后续值保存在通过rlink字段链接的各个节点中,
     * 各个节点的llink字段指向其前驱
     */
    struct Node {
        struct Node * llink;
        struct Node * rlink;
        void * value;
    } * head;

    /**
     * 元素的个数
     */
    int length;
};


/**
 * 创建并返回一个空环
 * @return
 */
T Ring_new(void) {
    T ring;
    NEW0(ring);
    ring->head = NULL;
    ring->length = 0;
    return ring;
}

/**
 * 创建并返回一个环,
 * 参数列表结束于第一个NULL指针参数,
 * 所以最后一个参数必须是NULL
 * @param x
 * @param ...
 * @return
 */
T Ring_ring(void * x, ...){
    va_list ap;
    T ring = Ring_new();

    va_start(ap, x);
    for(; x; x = va_arg(ap , void *)) {
        Ring_addhigh(ring, x);
    }
    va_end(ap);

    return ring;
}

/**
 * 释放*ring指定的环并将其清零
 * 如果ring或者*ring是NULL,则导致运行时错误
 *
 * @param ring
 */
void Ring_free(T * ring){
    struct Node * p;
    struct Node * q;

    assert(ring && *ring);

    //首先释放各个Node节点实例,
    if( (p = (*ring)->head) != NULL) {
        int n = (*ring)->length;
        for(; n-- > 0; p = q) {
            q = p->rlink;
            FREE(p);
        }
    }

    //然后释放Ring_T结构实例
    FREE(*ring);
}

/**
 * 返回ring中值的数目
 * @param ring  为NULL导致运行时错误
 * @return
 */
int Ring_length(T ring){
    assert(ring);
    return ring->length;
}

/**
 * q <--- ith node 136
 * 根据索引值找到指定的Node指针
 * @param ring
 * @param index
 * @return
 */
static struct Node * get_node(T ring, int index) {
    int n;
    struct Node * q;
    q = ring->head;

    if(index <= (ring->length / 2)) { //在右侧找
        for(n = index; n-- > 0;) {
            q = q->rlink;
        }
    } else { //在左侧找
        for(n = ring->length - index; n-- > 0;) {
            q = q->llink;
        }
    }

    return q;
}


/**
 * 返回index索引处的值
 * @param ring   为NULL导致运行时错误
 * @param index  小于0或者大于ring的值的长度导致运行时错误,即 取值为 0 ~ N-1
 * @return
 */
void * Ring_get(T ring, int index){
    struct Node * q;
    assert(ring);

    assert(index >= 0 && index < ring->length);

    q = get_node(ring, index);
    return q->value;
}

/**
 * 将ring中索引值为index的值修改为val,并返回原始值
 * @param ring     为NULL导致运行时错误
 * @param index    小于0或者大于ring的值的长度导致运行时错误 ,即 取值为 0 ~ N-1
 * @param val      这里可以为NULL,没有做检查
 * @return
 */
void * Ring_put(T ring, int index, void * val){
    struct Node * q;
    void * prev;
    assert(ring);
    assert(index >= 0 && index < ring->length);

    q = get_node(ring, index);

    prev = q->value;
    q->value = val;  //覆盖原来的值

    return prev;
}


/**
 * 将val值添加到ring中的pos位置处,并返回val
 * 在一个含有N个值的环中, pos位置指定了值之间的地点,
 * 添加一个新值,会将其右侧所有值的索引加1,并将环的长度加1.
 * 可能导致Mem_Failed
 * 如下, 给出了一个包含6个值的环的正负位置:
 *
 *
 *  1   2   3   4   5   6   7     <-- 这是正位置,pos, 取值 1 ~ (n+1)
 *  |   |   |   |   |   |   |
 *  V   V   V   V   V   V   V
 *  -------------------------
 *  | 0 | 1 | 2 | 3 | 4 | 5 |    <-- 这是索引值 index
 *  -------------------------
 *  ^   ^   ^   ^   ^   ^   ^
 *  |   |   |   |   |   |   |
 * -6  -5  -4  -3  -2  -1   0     <-- 这是负位置, pos, 取值 -n ~ 0
 *
 *
 * @param  ring  为NULL导致运行时错误
 * @param  pos   大于 N+1 或者小于-N 导致运行时错误
 * @param  val   这里可以为NULL,没有做检查
 * @return
 */
void * Ring_add(T ring, int pos , void * val) {
    assert(ring);
    assert(pos >= -ring->length && pos < ring->length + 1);

    //向环的两端添加值的情形,可以通过Ring_addlow(), Ring_addhigh()来处理, 空环的处理也涵盖在其中了
    if (pos == 1 || pos == -ring->length) {  //最低位置
        return Ring_addlow(ring, val);
    } else if(pos == 0 || pos == ring->length + 1) {  //最高位置
        return Ring_addhigh(ring, val);
    } else {

        struct Node * newNode;
        struct Node * q;

        //首先通过位置值得到该位置右侧的值的索引,
        //根据pos转换成索引值, 这个索引值位于插入点的右侧
        int i = pos < 0 ? pos + ring->length : pos - 1;

        q = get_node(ring, i); //

        NEW(newNode); //初始化新结点

        //然后将新结点添加到其左侧

        //insert p to the left of q 137
        newNode->llink = q->llink;  //新的结点的左边链接 指向 q的左边
        newNode->rlink = q;         //新的结点的右边链接 指向 q本身

        q->llink->rlink = newNode;  //原来的最左边的结点 的右链接 指向 新结点
        q->llink = newNode;         //q的左链接 指向 新结点

        ring->length ++;
        return newNode->value = val;
    }
}


/**
 * Ring_addlow()函数等效于 Ring_add(ring, 1, val)
 * @param ring
 * @param val
 * @return
 */
void * Ring_addlow(T ring, void * val){
    assert(ring);

    Ring_addhigh(ring, val); //先把新结点加到head的左边

    ring->head = ring->head->llink;  //然后把head向左移动到新结点位置, 则新结点变成了第一个位置

    return val;
}

/**
 * Ring_addhigh()等效于Ring_add(ring, 0, val)
 * @param ring
 * @param val
 * @return
 */
void * Ring_addhigh(T ring ,void * val){
    assert(ring);

    struct Node * p;
    struct Node * q;

    //首先必须分配一个Node结点,并初始化它
    NEW(p);

    if ((q = ring->head) != NULL) {
        //将其插入到双链表中正确的位置
        //这里只需要处理向head指向的结点左侧添加就行了
        //insert p to the left of  q 137

        p->llink = q->llink;  //新的结点的左边链接 指向 head的左边
        p->rlink = q;         //新的结点的右边链接 指向 head本身

        q->llink->rlink = p;  //原来的最左边的结点 的右链接 指向 新结点
        q->llink = p;         //head的左链接 指向 新结点

    } else {
        //还必须处理想空环中添加结点的情况
        //make p ring's only value 137
        ring->head = p->llink = p->rlink = p;
    }

    ring->length++;
    return p->value = val;
}

/**
 * 删除并返回ring中的索引为index的元素的值,
 * 删除之后,会将其右侧剩下的索引都减1, 并将环的长度减小1,
 *
 * @param ring    为NULL导致运行时错误
 * @param index   小于0或者大于ring的长度, 会导致运行时错误
 * @return
 */
void * Ring_remove(T ring, int index){
    assert(ring);
//    assert(ring->length > 0);
    if(ring->length == 0){
        return NULL;
    }
    assert(index >= 0 && index < ring->length);

    void * x;
    struct Node * q;

    q = get_node(ring, index);
    if (index == 0) {
        ring->head = ring->head->rlink;
    }

    x = q->value;

    //delete node q 139
    q->llink->rlink = q->rlink; //q结点的左节点的右链接指向 q的右节点
    q->rlink->llink = q->llink; //q结点的右节点的左链接指向 q的左节点

    FREE(q);
    if(--ring->length == 0) {  //长度为0时, head置空
        ring->head = NULL;
    }


    return x;
}

/**
 * 删除并返回位于ring的低端的值
 * 等效于 Ring_remove(ring, 0)
 *
 * @param ring  为NULL导致运行时错误
 * @return
 */
void * Ring_remlow(T ring){
    assert(ring);
    if (ring->length == 0 || ring->head == NULL) {
        return NULL;
    }

    ring->head = ring->head->rlink;  //head向右移动一个位置, 这时候最低位置就变成了最高位置了
    return Ring_remhigh(ring);      //然后调用函数删除最高位置
}

/**
 * 删除并返回位于ring的高端的值
 * 等效于 Ring_remove(ring, Ring_length(ring) - 1)
 *
 * @param ring  为NULL导致运行时错误
 * @return
 */
void * Ring_remhigh(T ring){
    assert(ring);
    if (ring->length == 0 || ring->head == NULL) {
        return NULL;
    }

    struct Node * q;
    void * x;

    q = ring->head->llink; //head的左边的结点
    x = q->value;          //head的左边的结点的值

    //delete node q 139
    q->llink->rlink = q->rlink; //q结点的左节点的右链接指向 q的右节点
    q->rlink->llink = q->llink; //q结点的右节点的左链接指向 q的左节点
    FREE(q);
    if (--ring->length == 0) {
        ring->head = NULL;
    }

    return x;
}

/**
 * 左旋或者右旋 ring, 将其中的值重新编号
 * 如果n为正值,ring则右旋转n个值(顺时针), 各个值的索引加n然后对ring的长度取模
 * 如果n为负值, ring向左旋转n个值(逆时针),各个值的索引值减n然后对环的长度取模.
 * 如果环的长度为0, 无效果
 * 如果n的绝对值大于ring的长度, 会导致运行时错误
 *
 * @param ring   为NULL导致运行时错误
 * @param n      大于 N+1 或者小于-N 导致运行时错误
 */
void Ring_rotate(T ring , int n){
    assert(ring);
    assert(n >= -ring->length && n <= ring->length);

    //根据n找到旋转之后的原点的索引值
    int index;
    if (n >= 0) {
        index = n % ring->length;
    } else {
        index = n + ring->length;
    }
    //找到这个索引值处的结点,把这个结点作为head的值,也就是原点
    struct Node * q = get_node(ring, index);
    ring->head = q;
}