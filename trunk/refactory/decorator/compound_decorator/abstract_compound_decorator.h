/*
 * abstract_compound_decorator.h
 *
 *  Created on: 2013-4-26
 *      Author: guoweijiang
 */

#ifndef ABSTRACT_COMPOUND_DECORATOR_H_
#define ABSTRACT_COMPOUND_DECORATOR_H_

#include "../single_decorator/abstract_single_decorator.h"

class AbstractCompoundDecorator : public AbstractDecorator
{
protected:

    // 查询附表时的条件，联合嵌套查询时使用
    vector<AbstractDecorator*> secondery_table_query;

    // 查询最终主表时的业务条件
    vector<AbstractDecorator*> primary_table_query;

public:

    AbstractCompoundDecorator();
    virtual ~AbstractCompoundDecorator();

    void add_primary_table_decorator(AbstractDecorator* decorator);
    void add_secondery_table_decorator(AbstractSingleDecorator* decorator);

	/* add by zhengxie */
	void remove_primary_table_decorator(AbstractSingleDecorator* decorator);
    void remove_secondery_table_decorator(AbstractSingleDecorator* decorator);
	void remove_all_primary_table_decorator();
	void remove_all_secondery_table_decorator();
    /*
     *  一般抽象接口：
     *
     *  一般简单的业务中，简单的嵌套所有查询条件即可
     *  一般抽象接口的实现中，会将所有查询条件简单的嵌套在一起
     *
     *  【注意】：
     *  如果有 is_special_query 逻辑时，需要将基本逻辑用 $query 嵌套起来
     *
     *  子类中需要根据具体业务需要，按照不同的要求进行相应的条件嵌套
     */
    virtual int custom_result_set_query(bson* query);

    /*
     *  AbstractCompoundDecorator 的子类中有可能要按照具体的业务逻辑再分别实现另外两个一般抽象接口：
     *  set_id_list_fetching_query 和  set_list_count_query
     *
     *  特别是  set_id_list_fetching_query, 因为如果涉及到需要查询两张表的时候
     *  set_id_list_fetching_query 和  custom_result_set_query 很有可能是需要不同的查询条件
     */
    virtual int set_id_list_fetching_query(bson* query);

    /*
     *  一般抽象接口：
     *
     *  由于在计算 count 时不支持 is_special_query(例如：orderby) 逻辑
     *  所以在 count 时要刨除这些逻辑，保留基本逻辑即可
     *  子类中需要根据具体业务需要，按照不同的要求进行相应的条件嵌套
     */
    virtual int get_count_query(bson* query);
};

#endif /* ABSTRACT_COMPOUND_DECORATOR_H_ */
