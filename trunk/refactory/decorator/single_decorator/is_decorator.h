/*
 * is_decorator.h
 *
 *  Created on: 2013-4-27
 *      Author: guoweijiang
 */

#ifndef IS_DECORATOR_H_
#define IS_DECORATOR_H_

#include "abstract_single_decorator.h"

class IsDecorator : public AbstractSingleDecorator
{
public:

    IsDecorator();
    virtual ~IsDecorator();

    /*
     *  实现抽象接口：
     */
    int custom_result_set_query(bson* query);
};

#endif /* IS_DECORATOR_H_ */
