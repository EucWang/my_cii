//
// Created by wangxn on 2019/11/25.
//

#ifndef ICD_ARRAYREP_H
#define ICD_ARRAYREP_H

#define T Array_T

/**
 * 如果数组没有元素, array字段为NULL,
 * 这个结构体是数组描述符, 信息矢量(dope vector)
 *
 * ArrayRep可以被读取,但是不能被修改,否则会导致运行时错误
 */
struct T {
    int length;
    int size;
    char * array;
};

/**
 * 初始化array指向的Array_T结构实例的各个字段,将其分别设置为参数length,size he ary的值
 *
 *
 * @param array
 * @param length
 * @param size
 * @param ary
 */
extern void ArrayRep_init(T array, int length, int size, void * ary);


#undef T
#endif //ICD_ARRAYREP_H
