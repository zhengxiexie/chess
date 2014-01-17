/*
 * orderby_decorator.h
 *
 *  专门处理 mongoDB 中对某几个字段执行 orderby 逻辑的装饰器
 *
 *  Created on: 2013-4-27
 *      Author: guoweijiang
 */

#ifndef ORDERBY_DECORATOR_H_
#define ORDERBY_DECORATOR_H_

#include "abstract_single_decorator.h"

class OrderbyDecorator : public AbstractSingleDecorator
{
public:

    // 排序类型： 1 升序 ; -1 降序
    int order_type;

    OrderbyDecorator();
    virtual ~OrderbyDecorator();

    /*
     *  实现抽象接口：
     */
    int custom_result_set_query(bson* query);
};


#endif /* ORDERBY_DECORATOR_H_ */
