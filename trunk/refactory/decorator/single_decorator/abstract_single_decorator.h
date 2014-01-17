/*
 * abstract_single_decorator.h
 *
 *  Created on: 2013-4-26
 *      Author: guoweijiang
 */

#ifndef ABSTRACTSINGLEDECORATOR_H_
#define ABSTRACTSINGLEDECORATOR_H_

#include "../abstract_decorator.h"

class AbstractSingleDecorator : public AbstractDecorator
{
protected:

    /*
     *  现在所有的过滤条件只支持 string 类型
     *  如果以后需要有其他类型的再做扩展即可
     */
    vector<MCPTupleDecoratorItem> fitler_conditions;

public:

    // 权重排序的类型
    WeightType weight_type;

    AbstractSingleDecorator();
    virtual ~AbstractSingleDecorator();

    void clear()
    {
        this->fitler_conditions.clear();
    }

    /*
     *  从一个已知的vector列表中获取查询条件，设置 fitler_conditions
     *  这个函数适用于针对同一个列名的多个限制
     */
    int set_same_field_filter_conditions(vector<const char*>* condition_list, // 已知条件数值
                                         const char* table_field_name,        // 参与数组判断的列名
                                         bool is_string_value);               // 参与判别的值是否为 string 类型

    /*
     *  设置  KEY:VALUE 条件对儿
     */
    void add_field_filter_condition(const char* key_name, const char* key_value);

    /*
     *  由于   this->fitler_conditions 中只保存指针
     *  所以传入的  value_entity 保存  key_value 的 string 类型实体
     *  this->fitler_conditions 指向这个实体
     */
    void add_field_filter_condition(const char* key_name,
                                    const int key_value,   // 参与判断的值
                                    string& value_entity); // 判断值转换成 string 类型的实体

    void push_back(MCPTupleDecoratorItem& decorator);

    /*
     *  抽象接口：
     *  用于装饰指定业务查询的条件，是装饰器的核心接口
     */
    virtual int custom_result_set_query(bson* query) = 0;
};

#endif /* ABSTRACTSINGLEDECORATOR_H_ */
