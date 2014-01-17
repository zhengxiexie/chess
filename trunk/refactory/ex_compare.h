/*
 * ex_compare.h
 *
 *  Created on: 2013-7-17
 *      Author: guoweijiang
 */

#ifndef EX_COMPARE_H_
#define EX_COMPARE_H_

#include "./common.h"

// 纯抽象接口类
class CompareTwoValues
{
    /*
     *  return:
     *      1  : front_value > rear_value
     *      0  : front_value = rear_value
     *      -1 : front_value < rear_value
     */
    virtual int compare(void* front_value, void* rear_value) = 0;
};

// 比较字符串的实现类(可以用于日期字符串的比较)
class StringValuesCompare : public CompareTwoValues
{
public:

    /*
     *   内部会将  void* 转换成 const char* 类型比较
     */
    int compare(void* front_value, void* rear_value);
};

// 排序接口
class SequenceProcessor
{
public:

    /*
     *   将  MAP 类型数据中指定的字段按指定顺序排序
     *   result_seq_key_list 是排序好的指定索引
     *
     *   TODO : 目前先采用简单插入排序算法，将来可用二分插入排序算法或其余更好的算法
     *
     *   return:
     *     0  : recource_data 为空
     *     -1 : 指定的 seq_field_name 不存在
     *     -2 : 指定的 key_field_name 不存在
     *     -3 : 排序过程中出现错误
     *     1  : 正常
     */
    int process_sequence(map<string, string>& recource_data,
                         list<const char*>& result_seq_key_list,
                         const char* seq_field_name,
                         const char* key_field_name,
                         CompareTwoValues* compare,
                         bool is_acs);
};
#endif /* EX_COMPARE_H_ */
