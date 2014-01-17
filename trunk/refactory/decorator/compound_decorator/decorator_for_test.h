/*
 * typed_class_decorator.h
 *
 *  Created on: 2013-4-26
 *      Author: guoweijiang
 */

#ifndef TYPED_CLASS_DECORATOR_H_
#define TYPED_CLASS_DECORATOR_H_

#include "abstract_compound_decorator.h"

class TypedClassDecorator : public AbstractCompoundDecorator
{
public:

    TypedClassDecorator();
    virtual ~TypedClassDecorator();

    int custom_result_set_query(bson* query);
};

#endif /* WEIGHT_CLASS_DECORATOR_H_ */
