/*
 * like_decorator.h
 *
 *  Created on: 2013-7-4
 *      Author: guoweijiang
 */

#ifndef LIKE_DECORATOR_H_
#define LIKE_DECORATOR_H_

#include "abstract_single_decorator.h"

class LikeDecorator : public AbstractSingleDecorator
{
public:

    LikeDecorator();
    virtual ~LikeDecorator();

    /*
     *  实现抽象接口：
     */
    int custom_result_set_query(bson* query);
};

#endif /* LIKE_DECORATOR_H_ */
