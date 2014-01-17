/*
 * smaller_decorator.h
 *
 *  Created on: 2013-6-3
 *      Author: guoweijiang
 */

#ifndef SMALLER_DECORATOR_H_
#define SMALLER_DECORATOR_H_

#include "abstract_single_decorator.h"

class SmallerThanDecorator : public AbstractSingleDecorator
{
public:

    bool is_smaller_and_equal;

    SmallerThanDecorator();
    virtual ~SmallerThanDecorator();

    /*
     *  实现抽象接口：
     */
    int custom_result_set_query(bson* query);
};

#endif /* SMALLER_DECORATOR_H_ */
