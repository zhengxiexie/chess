/*
 * extend_column.cpp
 *
 *  Created on: 2013-5-6
 *      Author: guoweijiang
 */

#include "extend_column.h"

ExMysqlColumn::ExMysqlColumn(MysqlTable* owner, const char* column_name, bool is_string_type)
{
    LOGD("[GWJ] %s:  start", __FUNCTION__);

    this->owner_table = owner;

    if(this->owner_table != NULL)
    {
        if(IS_VALUED_STRING(this->owner_table->nick_name))
        {
            this->column_name = string(this->owner_table->nick_name) + ".`"
                                + column_name + "`";
        }
        else
        {
            this->column_name = "`" + string(column_name) + "`";
        }

        LOGD("[GWJ] %s: Ex-column[%s]", __FUNCTION__, this->column_name.c_str());
    }
    else
    {
        LOGD("[GWJ] %s: NULL owner table ERROR!", __FUNCTION__);
    }

    this->order_type = NULL;
    this->is_group_by = false;
    this->is_string_value = is_string_type;
}

ExMysqlColumn::~ExMysqlColumn()
{
    this->clear();
}

void ExMysqlColumn::clear()
{
    this->column_name.clear();
    this->column_value.clear();

    int size = this->where_condition.size();

    for(int i = 0; i < size; i++)
    {
        this->where_condition[i].clear();
    }

    this->where_condition.clear();

    size = this->join_conditon.size();

    for(int i = 0; i < size; i++)
    {
        this->join_conditon[i].clear();
    }

    this->join_conditon.clear();
}

/*
 *  判断传入的 col_name 是否为此 column_name
 */
bool ExMysqlColumn::is_column_name(const char* col_name)
{
    const char* pure_this_col_name;
    const char* pure_col_name;
    string pure_column;

    if(IS_VALUED_STRING(this->owner_table->nick_name))
    {
        pure_this_col_name = CommonInterface::find_pure_column_name(this->column_name.c_str());
    }
    else
    {
        pure_this_col_name = this->column_name.c_str();
    }

    if(IS_VALUED_STRING(col_name))
    {
        pure_col_name = CommonInterface::find_pure_column_name(col_name);
    }
    else
    {
        LOGD("[GWJ] %s: Error input [col_name]", __FUNCTION__);
        return false;
    }

    if(!IS_VALUED_STRING(pure_col_name))
    {
        LOGD("[GWJ] %s: [col_name:%s] is pure itself", __FUNCTION__, col_name);
        pure_column = string("`") + col_name + "`";
        pure_col_name = pure_column.c_str();
    }

    if(pure_this_col_name == NULL ||
            pure_col_name == NULL)
    {
        LOGD("[GWJ] %s: Error input [col_name:%s] or [Column_name:%s]",
             __FUNCTION__, pure_col_name, pure_this_col_name);

        return false;
    }

    if(strcmp(pure_this_col_name, pure_col_name) == 0)
    {
        LOGD("[GWJ] %s: Same name[%s][%s]",
             __FUNCTION__, pure_col_name, pure_this_col_name);

        return true;
    }
    else
    {
        LOGD("[GWJ] %s: Different name[%s][%s]",
             __FUNCTION__, pure_col_name, pure_this_col_name);

        return false;
    }
}

const char* ExMysqlColumn::get_entire_column_name()
{
    return this->column_name.c_str();
}

/*
 *  设置与此 Column 联合的 JOIN 表
 *  一般来说只设置  ON column 用于连接表即可
 *  如果有特殊要求，则会输入扩展条件  ext_condition
 *
 *  实验显示，一般情况下   ext_condition 条件续放到外面才起作用
 */
void ExMysqlColumn::add_join(const char* type,
                             MysqlTable* join_table,
                             const char* on_column,
                             const char* ext_condition)
{
    if(type == NULL || join_table == NULL || on_column == NULL)
    {
        LOGD("[GWJ] %s: [join type] or [join table] or [ON column] NULL ERROR!! ", __FUNCTION__);
        return;
    }

    JoinCondition join_condition(type, join_table, on_column);

    if(ext_condition != NULL)
    {
        join_condition.set_extra(ext_condition);
    }

    /*
     *  当执行 join 操作时，要把参与 joint 的表需要 select 的列加入到 owner 表中
     *  保证在执行 SQL 语句时能够选出 joint 表中想要的列
     */
    MysqlTable* this_table = this->owner_table;
    int joint_col_size = join_table->get_select_column_size();

    for(int i = 0; i < joint_col_size; i++)
    {
        const char* joint_column = (*join_table)[i];

        if(joint_column != NULL)
        {
            this_table->add_select_column(joint_column, join_table->nick_name);
        }
        else
        {
            LOGD("[GWJ] %s: NULL joint column[%d]", __FUNCTION__, i);
        }
    }

    this_table->add_join_table(join_table);
    this->join_conditon.push_back(join_condition);

    LOGD("[GWJ] %s:add join[%s %s]", __FUNCTION__, type, join_table->table_name);
}

void ExMysqlColumn::add_left_join(MysqlTable* join_table,
                                  const char* on_column,
                                  const char* extra_condition)
{
    if(join_table == NULL || on_column == NULL)
    {
        LOGD("[GWJ] %s: LEFT JOIN [join table] or [ON column] NULL ERROR!! ", __FUNCTION__);
        return;
    }

    this->add_join("LEFT JOIN", join_table, on_column, extra_condition);
}

void ExMysqlColumn::add_right_join(MysqlTable* join_table,
                                   const char* on_column,
                                   const char* extra_condition)
{
    if(join_table == NULL || on_column == NULL)
    {
        LOGD("[GWJ] %s: RIGHT JOIN [join table] or [ON column] NULL ERROR!! ", __FUNCTION__);
        return;
    }

    this->add_join("RIGHT JOIN", join_table, on_column, extra_condition);
}

void ExMysqlColumn::add_inner_join(MysqlTable* join_table,
                                   const char* on_column,
                                   const char* extra_condition)
{
    if(join_table == NULL || on_column == NULL)
    {
        LOGD("[GWJ] %s: INNER JOIN [join table] or [ON column] NULL ERROR!! ", __FUNCTION__);
        return;;
    }

    this->add_join("INNER JOIN", join_table, on_column, extra_condition);
}

void ExMysqlColumn::add_condition(const char* type,
                                  const char* operation,
                                  const char* value)
{
    if(type == NULL || value == NULL)
    {
        LOGD("[GWJ] %s: [condition type] or [value] NULL ERROR!! ", __FUNCTION__);
        return;
    }

    WhereCondition condition(type, operation);
    condition.set_value(value);
    this->where_condition.push_back(condition);
}

void ExMysqlColumn::add_condition(const char* type,
                                  const char* operation,
                                  const int value)
{
    if(type == NULL)
    {
        LOGD("[GWJ] %s: [condition type] NULL ERROR!! ", __FUNCTION__);
        return;
    }

    WhereCondition condition(type, operation);
    condition.set_value(value);
    this->where_condition.push_back(condition);
}

void ExMysqlColumn::add_and_after_now()
{
    this->is_string_value = false;
    this->add_condition("AND", " > ", " now() ");
}

void ExMysqlColumn::add_and_before_now()
{
    this->is_string_value = false;
    this->add_condition("AND", " < ", " now() ");
}

void ExMysqlColumn::add_or_after_now()
{
    this->is_string_value = false;
    this->add_condition("OR", " > ", " now() ");
}

void ExMysqlColumn::add_or_before_now()
{
    this->is_string_value = false;
    this->add_condition("OR", " < ", " now() ");
}

void ExMysqlColumn::add_or_condition(const char* operation,
                                     const char* value)
{
    this->is_string_value = true;
    this->add_condition("OR", operation, value);
}

void ExMysqlColumn::add_or_condition(const char* operation,
                                     const int value)
{
    this->is_string_value = false;
    this->add_condition("OR", operation, value);
}

void ExMysqlColumn::add_or_condition(const char* operation,
                                     ExMysqlColumn& column)
{
    this->is_string_value = false;

    this->add_condition("OR", operation,
                        column.get_entire_column_name());
}

void ExMysqlColumn::add_or_conditions(const char* operation,
                                      vector<const char*>& values)
{
    this->is_string_value = true;

    int size = values.size();

    for(int i = 0 ; i < size; i++)
    {
        LOGD("[GWJ] %s: or %dth [%s]", __FUNCTION__, i, values[i]);
        this->add_condition("OR", operation, values[i]);
    }
}

void ExMysqlColumn::add_or_conditions(const char* operation,
                                      vector<int>& values)
{
    this->is_string_value = false;

    int size = values.size();

    for(int i = 0 ; i < size; i++)
    {
        LOGD("[GWJ] %s: or %dth [%d]", __FUNCTION__, i, values[i]);
        this->add_condition("OR", operation, values[i]);
    }
}

void ExMysqlColumn::add_and_condition(const char* operation,
                                      const char* value)
{
    this->is_string_value = true;
    this->add_condition("AND", operation, value);
}

void ExMysqlColumn::add_and_condition(const char* operation,
                                      const int value)
{
    this->is_string_value = false;
    this->add_condition("AND", operation, value);
}

void ExMysqlColumn::add_and_condition(const char* operation,
                                      ExMysqlColumn& column)
{
    this->is_string_value = false;
    this->add_condition("AND", operation,
                        column.get_entire_column_name());
}

void ExMysqlColumn::add_and_conditions(const char* operation,
                                       vector<const char*>& values)
{
    this->is_string_value = true;

    int size = values.size();

    for(int i = 0 ; i < size; i++)
    {
        LOGD("[GWJ] %s: and %dth [%s]", __FUNCTION__, i, values[i]);
        this->add_condition("AND", operation, values[i]);
    }
}

void ExMysqlColumn::add_and_conditions(const char* operation,
                                       vector<int>& values)
{
    this->is_string_value = false;

    int size = values.size();

    for(int i = 0 ; i < size; i++)
    {
        LOGD("[GWJ] %s: and %dth [%d]", __FUNCTION__, i, values[i]);
        this->add_condition("AND", operation, values[i]);
    }
}

void ExMysqlColumn::add_orderby_decrease()
{
    this->order_type = "DESC";
}


void ExMysqlColumn::add_orderby_acrease()
{
    this->order_type = "ASC";
}

/*
 *  等号运算符重载只应用于设置列值
 *  在 INSERT / UPDATE 时使用
 */
void ExMysqlColumn::set_value(const char* value)
{
    MCPTupleItem tuple_condition;
    tuple_condition.key = this->column_name;
    tuple_condition.value = value;
    tuple_condition.is_string_value = true;

    this->owner_table->insert_update_conditions.push_back(tuple_condition);
}

/*
 *  等号运算符重载只应用于设置列值
 *  在 INSERT / UPDATE 时使用
 */
void ExMysqlColumn::set_value(int value)
{
    char value_str[10];
    memset(value_str, 0x00, sizeof(value_str));
    sprintf(value_str, "%d", value);

    MCPTupleItem tuple_condition;
    tuple_condition.key = this->column_name;
    tuple_condition.value = value_str;
    tuple_condition.is_string_value = false;

    this->owner_table->insert_update_conditions.push_back(tuple_condition);
}

/*
 *  当有多个 Ex-column 进行 where 语句连接时
 *  this->where_condition[0].where_type 是用来连接  Ex-column 间的 and / or 逻辑的
 *  这个接口作用是返回连接 Ex-column 间 and / or 逻辑
 *
 *  this->where_condition[i].where_type(i>0)
 *  后面的这些 where_type 就是连接这个 Ex-column 内部 and / or 逻辑的
 */
const char* ExMysqlColumn::get_header_where_type()
{
    if(this->where_condition.size() > 0 &&
       IS_VALUED_STRING(this->where_condition[0].where_type))
    {
        LOGD("[GWJ] %s: Header where condition[%s]",
                 __FUNCTION__, this->where_condition[0].where_type);

        return this->where_condition[0].where_type;
    }
    else
    {
        LOGD("[GWJ] %s: Error header where condition. NULL or Empty", __FUNCTION__);
        return NULL;
    }
}

/*
 *  获取关于此 Ex-column 的 WHERE 语句
 *  没有 WHERE 语句时返回  false
 */
bool ExMysqlColumn::get_where_condition_query(string& query)
{
    if(!IS_VALUED_STRING(this->column_name.c_str()))
    {
        LOGD("[GWJ] %s: Ex-Column Name Empty Error!", __FUNCTION__);
        return false;
    }

    int size = this->where_condition.size();

    if(size == 0)
    {
        LOGD("[GWJ] %s: No WHERE SQL", __FUNCTION__);
        return false;
    }

    LOGD("[GWJ] %s:  start. where size[%d]", __FUNCTION__, size);

    for(int i = 0; i < size; i++)
    {
        string cur_condition;

        /*
         * 如果是 string 类型，value 要被 "value" 包含
         * 如果不是 string 类型，则直接连接 value 即可
         */
        if(this->is_string_value)
        {
            cur_condition = this->column_name + this->where_condition[i].operation + " \"" + this->where_condition[i].value + "\" ";
        }
        else
        {
            cur_condition = this->column_name + this->where_condition[i].operation + this->where_condition[i].value;
        }

        if(i > 0)
        {
            /*
             *  i = 0 的那个 where_condition[0].where_type 是留给外层连接 this->where_condition 用的
             *  在 this->where_condition 内部用不上  where_condition[0].where_type
             */
            query = query + string(this->where_condition[i].where_type) + " " + cur_condition + " ";
        }
        else
        {
            query = string(" ") + cur_condition + " ";
        }

        LOGD("[GWJ] %s: Current where[%s]",
             __FUNCTION__, cur_condition.c_str());
    }

    query = string("(") + query + ")";

    LOGD("[GWJ] %s: end.where[%s]", __FUNCTION__, query.c_str());
    return true;
}

/*
 *  获取关于此 Ex-column 的 JOIN 语句
 *  如果没有 JOIN 语句则返回 false
 */
bool ExMysqlColumn::get_join_query(string& query)
{
    if(!IS_VALUED_STRING(this->column_name.c_str()))
    {
        LOGD("[GWJ] %s: Ex-Column Name Error!", __FUNCTION__);
        return false;
    }

    int size = this->join_conditon.size();

    if(size == 0)
    {
        LOGD("[GWJ] %s: No Join SQL", __FUNCTION__);
        return false;
    }

    LOGD("[GWJ] %s:  start. join size[%d]", __FUNCTION__, size);

    for(int i = 0; i < size; i++)
    {
        string cur_join;
        string cur_join_table;
        string cur_join_condition;

        if(this->join_conditon[i].join_table == NULL ||
           !IS_VALUED_STRING(this->join_conditon[i].join_type)  )
        {
            LOGD("[GWJ] %s:  start. join [%d] [table] or [type] NULL or Empty ERROR!!", __FUNCTION__, i);
            return false;
        }

        const char* joint_nick_name  = this->join_conditon[i].join_table->nick_name;
        const char* joint_table_name = this->join_conditon[i].join_table->table_name;
        const char* joint_on_column  = this->join_conditon[i].on_column;
        const char* joint_ext_cond   = this->join_conditon[i].extra_condition.c_str();

        if(!IS_VALUED_STRING(joint_table_name))
        {
            LOGD("[GWJ] %s: join [%d] [joint_table_name] NULL or Empty ERROR!!", __FUNCTION__, i);
            return false;
        }

        if(IS_VALUED_STRING(joint_nick_name))
        {
            cur_join_table = string(this->join_conditon[i].join_type) + " " +
                             joint_table_name + " AS " + joint_nick_name + " ON ";

            cur_join_condition = string(joint_nick_name) + "." +  joint_on_column  + " = " +
                                 this->column_name;
        }
        else
        {
            cur_join_table = string(this->join_conditon[i].join_type) +
                             " " + joint_table_name + " ON ";

            cur_join_condition = string(joint_on_column)  + " = " + this->column_name;
        }

        if(IS_VALUED_STRING(joint_ext_cond))
        {
            cur_join_condition = cur_join_condition + " AND " + joint_ext_cond;
        }

        cur_join = cur_join_table + "(" + cur_join_condition + ")";

        LOGD("[GWJ] %s: Cur join [%d]:%s", __FUNCTION__, i, cur_join.c_str());
        query = query + cur_join;
    }

    LOGD("[GWJ] %s: Join SQL %s", __FUNCTION__, query.c_str());
    return true;
}

/*
 *  构建 Order By 语句，如果没有则返回 false
 */
bool ExMysqlColumn::get_order_by_query(string& order_query)
{
    if(IS_VALUED_STRING(this->order_type))
    {
        if(IS_VALUED_STRING(this->column_name.c_str()))
        {
            order_query = " " + this->column_name + " " + this->order_type;
        }
        else
        {
            LOGD("[GWJ] %s:Owner Ex-Column Name NULL Error!", __FUNCTION__);
            return false;
        }
    }
    else
    {
        LOGD("[GWJ] %s:Ex-Column Name has no OrderBy!", __FUNCTION__);
        return false;
    }


    LOGD("[GWJ] %s: ret[%s]", __FUNCTION__, order_query.c_str());
    return true;
}

/*
 *  构建 Group By 语句，如果没有则返回 false
 */
bool ExMysqlColumn::get_group_by_query(string& query)
{
    if(this->is_group_by)
    {
        if(IS_VALUED_STRING(this->column_name.c_str()))
        {
            query = " " + this->column_name;
        }
        else
        {
            LOGD("[GWJ] %s:Owner Ex-Column Name NULL Error!", __FUNCTION__);
            return false;
        }
    }
    else
    {
        LOGD("[GWJ] %s:Ex-Column Name has no OrderBy!", __FUNCTION__);
        return false;
    }


    LOGD("[GWJ] %s: ret[%s]", __FUNCTION__, query.c_str());
    return true;
}


