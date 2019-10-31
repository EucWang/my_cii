//
// Created by wangxn on 2019/10/28.
//

#ifndef ICD_TABLE_H
#define ICD_TABLE_H

/**
 * 这里的Table_T结构对外隐藏了
 */
#define  T Table_T
typedef struct T * T;

/**
 * 可能引发Mem_Failed异常
 * @param hint  用于估计新的表中预期会容纳的表项数量,无论hint值如何,所有表都可以容纳任意数目的表项 , 优化去掉了这个参数
 * @param cmp   操作键值  比较函数
 * @param hash  操作键值  hash函数
 * @return
 */
extern T Table_new(int cmp(const void * x, const void * y), unsigned hash(const void * key));

/**
 * 释放table, 并将其设置为NULL,
 * Table_free并不释放键或者值
 * @param table
 */
extern void Table_free(T * table);

/**
 *
 * @param table
 * @return 返回表中键的数目
 */
extern int Table_length(T table);

/**
 * 添加一个新的键值对
 * @param table
 * @param key
 * @param value
 * @return
 */
extern void * Table_put(T * table, const void *key, void * value);

/**
 * 获取Table中对应key 的值
 * @param table
 * @param key
 * @return
 */
extern void * Table_get(T table, const void *key);

/**
 * 移除Table中某一个键值对
 * @param talbe
 * @param key
 * @return
 */
extern void * Table_remove(T talbe, const void * key);

/**
 * 遍历Table, 针对每一个键值对,调用apply函数,
 * @param table
 * @param apply
 * @param cl
 */
extern void Table_map(T table, void apply(const void *key, void ** value, void * cl), void * cl);

/**
 * 遍历Table,将键和值全部放入到数组中返回
 * 包含N个键值对的Table,本函数会构建一个有2N+1个元素的数组,并返回第一个元素的指针,
 * 在数组中键和值会交替出现,
 * 可能会引发Mem_failed异常
 * @param table
 * @param end
 * @return
 */
extern void ** Table_toArray(T table, void * end);

#undef T
#endif //ICD_TABLE_H
