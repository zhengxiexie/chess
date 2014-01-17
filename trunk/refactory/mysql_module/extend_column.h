/*
 * extend_column.h
 *
 *  Created on: 2013-5-6
 *      Author: guoweijiang
 */

#ifndef EXTEND_COLUMN_H_
#define EXTEND_COLUMN_H_

#include "../common.h"
#include "mysql_general_module.h"

class MysqlTable;

class WhereCondition
{

// 定义 ExMysqlColumn 为  WhereCondition 的友元类
friend class ExMysqlColumn;

protected:

    // 区分 and / or
    const char* where_type;

    // > < =
    const char* operation;

    string value;

public:
    WhereCondition(const char* type,
                   const char* operation)
    {
        LOGD("[GWJ] %s: new where type[%s], oper[%s]",
             __FUNCTION__, type, operation);

        this->where_type = type;
        this->operation = operation;
    }

    ~WhereCondition()
    {
        this->clear();
    }

    void clear()
    {
        this->value.clear();
    }

    void set_value(const char* value)
    {
        this->value = string(value);
    }

    void set_value(const int value)
    {
        char value_str[MYSQL_INT_VALUE_LEN];
        memset(value_str, 0x00, sizeof(value_str));
        sprintf(value_str, "%d", value);
        this->value = string(value_str);
    }
};


class JoinCondition
{

// 定义 ExMysqlColumn 为  JoinCondition 的友元类
friend class ExMysqlColumn;

protected:

    // 区分 left join / right join / inner join
    const char* join_type;

    /*
     *  参与 join 时的 ON 条件列
     */
    const char* on_column;

    /*
     *  进行 join 操作时 ON 语句可能也是一个组合逻辑
     *  这个变量用于保存这些多余的组合逻辑
     */
    string extra_condition;

    // 参与 join 的表
    MysqlTable* join_table;

public:

    JoinCondition(const char* type,
                  MysqlTable* join_table,
                  const char* on_column)
    {
        /*
         *  Join 操作时
         *  join_type , join_table 和  ON column 是必须的
         */
        this->join_type = type;
        this->join_table = join_table;
        this->on_column = on_column;
        this->extra_condition = "";
    }

    ~JoinCondition()
    {
        this->clear();
    }

    void clear()
    {
        this->extra_condition.clear();
    }

    void set_extra(const char* ex_cond)
    {
        this->extra_condition = ex_cond;
    }
};

class ExMysqlColumn
{

// 定义 MysqlTable 为 ExMysqlColumn 的友元类
friend class MysqlTable;

protected:

    // 该列所属的表
    MysqlTable* owner_table;

    /*
     *  如果有 nick_name 此处就是  nick_name.column_name 的形式
     */
    string column_name;

    /*
     *  column_value 将来用于 INSERT 语句
     */
    string column_value;

    bool is_string_value;
    bool is_group_by;

    vector<WhereCondition> where_condition;
    vector<JoinCondition> join_conditon;

    const char* order_type;

    void add_condition(const char* type,
                       const char* operation,
                       const char* value);

    void add_condition(const char* type,
                       const char* operation,
                       const int value);

    /*
     *  设置与此 Column 联合的 JOIN 表
     *  一般来说只设置  ON column 用于连接表即可
     *  如果有特殊要求，则会输入扩展条件  ext_condition
     *
     *  实验显示，一般情况下   ext_condition 条件续放到外面才起作用
     */
    void add_join(const char* type,
                  MysqlTable* join_table,
                  const char* on_column,
                  const char* ext_condition);

    /*
     *  判断传入的 col_name 是否为此 column_name
     */
    bool is_column_name(const char* col_name);

public:
    ExMysqlColumn(MysqlTable* owner,
                  const char* column_name,
                  bool is_string_type = true);

    ~ExMysqlColumn();

    void clear();

    /*
     *  获取完整的列名，如果有匿名则包含匿名
     */
    const char* get_entire_column_name();

    /*
     *  当有多个 Ex-column 进行 where 语句连接时
     *  this->where_condition[0].where_type 是用来连接  Ex-column 间的 and / or 逻辑的
     *  这个接口作用是返回连接 Ex-column 间 and / or 逻辑
     *
     *  this->where_condition[i].where_type(i>0)
     *  后面的这些 where_type 就是连接这个 Ex-column 内部 and / or 逻辑的
     */
    const char* get_header_where_type();

    /*
     *  获取关于此 Ex-column 的 WHERE 语句
     *  没有 WHERE 语句时返回  false
     */
    bool get_where_condition_query(string& query);

    /*
     *  获取关于此 Ex-column 的 JOIN 语句
     *  如果没有 JOIN 语句则返回 false
     */
    bool get_join_query(string& query);

    bool get_order_by_query(string& order_query);
    bool get_group_by_query(string& query);

    void add_and_after_now();
    void add_and_before_now();

    void add_or_after_now();
    void add_or_before_now();


    void add_or_condition(const char* operation,
                          const char* value);

    void add_or_condition(const char* operation,
                          const int value);

    void add_or_condition(const char* operation,
                          ExMysqlColumn& column);

    void add_or_conditions(const char* operation,
                           vector<const char*>& values);

    void add_or_conditions(const char* operation,
                           vector<int>& values);

    void add_and_condition(const char* operation,
                           const char* value);

    void add_and_condition(const char* operation,
                           const int value);

    void add_and_condition(const char* operation,
                           ExMysqlColumn& column);

    void add_and_conditions(const char* operation,
                            vector<const char*>& values);

    void add_and_conditions(const char* operation,
                            vector<int>& values);

    void add_left_join(MysqlTable* join_table,
                       const char* on_column,
                       const char* extra_condition);

    void add_right_join(MysqlTable* join_table,
                        const char* on_column,
                        const char* extra_condition);

    void add_inner_join(MysqlTable* join_table,
                        const char* on_column,
                        const char* extra_condition);

    void add_orderby_decrease();
    void add_orderby_acrease();

    /*
     *  等号运算符重载只应用于设置列值
     *  在 INSERT / UPDATE 时使用
     */
    void set_value(const char* value);
    void set_value(int value);
};


#endif /* EXTEND_COLUMN_H_ */
