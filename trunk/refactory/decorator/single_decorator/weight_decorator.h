/*
 * weight_decorator.h
 *
 *  专门处理按照指定权重排序的装饰器
 *
 *  Created on: 2013-4-26
 *      Author: guoweijiang
 */

#ifndef WEIGHT_DECORATOR_H_
#define WEIGHT_DECORATOR_H_

#include "abstract_single_decorator.h"

class OrderTypeStruct
{
public:
    const char* order_type;
    map<string, const char*> extra_tuple;
    AbstractDecorator* outer_decorator;
};

class WeightDecorator : public AbstractSingleDecorator
{
private:
    void order_by_extra_id(bson* command);

public:

    char* defalt_orderby;

    /*
     *  当按照  mcp_content 表的  class.p, column.p 排序时
     *  需要一种特殊处理，需要设置 class.id 和  column.id
     */
    const char* extra_id;

    /*
     *  如果是执行 aggregate 就不能按照一般顺序执行
     */
    AbstractDecorator* outer_decorator;

    WeightDecorator();
    virtual ~WeightDecorator();

    /*
     *   一般情况下设置为 NULL
     */
    WeightType get_weight_type(OrderTypeStruct& order_struct);

    /*
     *  实现抽象接口：
     */
    int custom_result_set_query(bson* query);

    /*
     *  实现一般抽象接口
     */
    int get_count_query(bson* query);
};

#endif /* WEIGHT_CLASS_DECORATOR_H_ */
