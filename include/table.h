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
 *
 * @param hint  用于估计新的表中预期会容纳的表项数量,无论hint值如何,所有表都可以容纳任意数目的表项
 * @param cmp   操作键值
 * @param hash  操作键值
 * @return
 */
extern T Table_new(int hint, int cmp(const void * x, const void * y), unsigned hash(const void * key));

/**
 *
 * @param table
 */
extern void Table_free(T * table);

#undef T
#endif //ICD_TABLE_H
