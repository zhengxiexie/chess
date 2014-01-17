/*
 * bigger_decorator.h
 *
 *  Created on: 2013-5-31
 *      Author: guoweijiang
 */

#ifndef BIGGER_DECORATOR_INT_H_
#define BIGGER_DECORATOR_INT_H_

#include "abstract_single_decorator.h"

class BiggerThanIntDecorator : public AbstractSingleDecorator
{
public:

    bool is_bigger_and_equal;

    BiggerThanIntDecorator();
    virtual ~BiggerThanIntDecorator();

    /*
     *  实现抽象接口：
     */
    int custom_result_set_query(bson* query);
};

#endif /* BIGGER_DECORATOR_INT_H_ */
