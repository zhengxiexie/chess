/*
 * not_equal_decorator.h
 *
 *  Created on: 2013-7-11
 *      Author: guoweijiang
 */

#ifndef NOT_EQUAL_DECORATOR_H_
#define NOT_EQUAL_DECORATOR_H_

#include "abstract_single_decorator.h"

class NotEqualDecorator : public AbstractSingleDecorator
{
public:

    NotEqualDecorator();
    virtual ~NotEqualDecorator();

    /*
     *  实现抽象接口：
     */
    int custom_result_set_query(bson* query);
};

#endif /* NOT_EQUAL_DECORATOR_H_ */
