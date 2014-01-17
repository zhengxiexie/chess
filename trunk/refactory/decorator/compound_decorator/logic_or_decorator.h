/*
 * logic_or_decorator.h
 *
 *  Created on: 2014-1-16
 *      Author: guoweijiang
 */

#ifndef LOGIC_OR_DECORATOR_H_
#define LOGIC_OR_DECORATOR_H_

#include "abstract_compound_decorator.h"

class LogicOrDecorator : public AbstractCompoundDecorator
{
public:
    LogicOrDecorator();
    virtual ~LogicOrDecorator();

    virtual int custom_result_set_query(bson* query);
    virtual int get_count_query(bson* query);
};

#endif /* LOGIC_OR_DECORATOR_H_ */
