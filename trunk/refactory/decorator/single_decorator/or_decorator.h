/*
 * or_decorator.h
 *
 *  专门处理 mongoDB 中对某一个字段执行 or 逻辑的装饰器
 *
 *  Created on: 2013-4-27
 *      Author: guoweijiang
 */

#ifndef OR_DECORATOR_H_
#define OR_DECORATOR_H_

#include "abstract_single_decorator.h"

class OrDecorator : public AbstractSingleDecorator
{
public:

    OrDecorator();
    virtual ~OrDecorator();

    /*
     *  实现抽象接口：
     */
    int custom_result_set_query(bson* query);
};

#endif /* IN_DECORATOR_H_ */
