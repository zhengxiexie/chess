/*
 * mysql_general_module.cpp
 *
 *  Created on: 2013-5-3
 *      Author: guoweijiang
 */

#include "mysql_general_module.h"

MysqlTable::MysqlTable(const char* table_name)
{
    this->key_name = NULL;
    this->table_name = table_name;
    this->nick_name = NULL;
    this->select_fields.clear();

    this->_unit_num = 10;
    this->_unit_times = 1;
    this->_start_pos = this->_unit_num * (this->_unit_times - 1);
    this->total_results = 0;

    LOGD("[GWJ] %s: end. unit_num[%d], unit_times[%d], start_pos[%d]",
            __FUNCTION__, this->_unit_num, this->_unit_times, this->_start_pos);
}

MysqlTable::MysqlTable(const char* table_name,
                       const char* nick_name)
{
    this->key_name = NULL;
    this->table_name = table_name;
    this->nick_name = nick_name;
    this->select_fields.clear();

    this->_unit_num = 10;
    this->_unit_times = 1;
    this->_start_pos = this->_unit_num * (this->_unit_times - 1);
    this->total_results = 0;

    LOGD("[GWJ] %s: end. unit_num[%d], unit_times[%d], start_pos[%d]",
            __FUNCTION__, this->_unit_num, this->_unit_times, this->_start_pos);
}

MysqlTable::~MysqlTable()
{
    LOGD("[GWJ] %s: start", __FUNCTION__);

    int size = this->extend_columns.size();

    for(int i = 0; i < size; i++)
    {
        this->extend_columns[i].clear();
    }

    this->extend_columns.clear();
    this->joint_tables.clear();

    size = this->select_fields.size();

    for(int i = 0; i < size; i++)
    {
        this->select_fields[i].clear();
    }

    size = this->id_s.size();

    for(int i = 0; i < size; i++)
    {
        this->result_set[this->id_s[i]].clear();
    }

    this->id_s.clear();
    this->result_set.clear();

    LOGD("[GWJ] %s: end", __FUNCTION__);
}

int MysqlTable::get_select_column_size()
{
    return this->select_fields.size();
}

/*
 *  检查一个列名是否已经是  nick.column 的形式
 */
bool MysqlTable::is_had_nick_name(const char* column)
{
    int size = strlen(column);

    for(int i = 0; i < size; i++)
    {
        if(column[i] == '.')
        {
            LOGD("[GWJ] %s: [%s] Already nick name",
                 __FUNCTION__, column);

            return true;
        }
    }

    LOGD("[GWJ] %s: [%s] Not nick name",
         __FUNCTION__, column);

    return false;
}

/*
 *  判断是否需要将传入的列名转换为  nick_name.`column_name` 的形式
 *  按照此对象内部的形式，统一格式
 *
 *  return: -1 错误 ; 0 不需要转换 ; 1 需要转换
 */
int MysqlTable::change_to_table_nick_column(const char* value, string& result_name)
{
    LOGD("[GWJ] %s: start. column_name[%s]",
         __FUNCTION__, value);

    if(IS_VALUED_STRING(value))
    {
        const char* pure = CommonInterface::find_pure_column_name(value);

        if(pure == NULL)
        {
            LOGD("[GWJ] %s: start. column_name[%s]. Not Pure Format",
                 __FUNCTION__, value);

            // 不是 nick_name.`column_name` 的格式，查看是否需要转换
            if(IS_VALUED_STRING(this->nick_name))
            {
                // 检查是否是 `column_name` 标准格式
                string standard_column;
                int is_standard = CommonInterface::is_standard_column_format(value, standard_column);

                if(is_standard < 0)
                {
                    LOGD("[GWJ] %s: start. column_name[%s]. Error Format!!",
                         __FUNCTION__, value);

                    return -1;
                }

                result_name = string(this->nick_name) + "." + standard_column;

                LOGD("[GWJ] %s: end. Result column[%s]", __FUNCTION__, result_name.c_str());
                return 1;
            }
            else
            {
                // 没有设置匿名，只检查是否是标准模式即可
                int is_standard = CommonInterface::is_standard_column_format(value, result_name);

                if(is_standard < 0)
                {
                    LOGD("[GWJ] %s: start. column_name[%s]. Error Format!!",
                            __FUNCTION__, value);

                    return -1;
                }

                LOGD("[GWJ] %s: end. Result column[%s]", __FUNCTION__, result_name.c_str());
                return 0;
            }
        }
        else
        {
            /*
             *  本身已经是 nick_name.column_name
             *  检查是否需要转换成 nick_name.`column_name` 的格式
             */
            string nick_name;
            int has_nick = CommonInterface::get_nick_name(value, nick_name);

            if(has_nick < 0)
            {
                LOGD("[GWJ] %s: start. column_name[%s]. Error Format!!",
                        __FUNCTION__, value);

                return -1;
            }

            string standard;
            int is_standard = CommonInterface::is_standard_column_format(pure, standard);

            if(is_standard < 0)
            {
                LOGD("[GWJ] %s: start. column_name[%s]. Error Format!!",
                        __FUNCTION__, value);

                return -1;
            }

            LOGD("[GWJ] %s: column[%s] has Nick Name Already. Pure[%s], Standard[%s], Nick_name[%s]",
                 __FUNCTION__, value, pure, standard.c_str(), nick_name.c_str());

            result_name = nick_name + "." + standard;

            LOGD("[GWJ] %s: end. Result column[%s]", __FUNCTION__, result_name.c_str());
            return 0;
        }
    }
    else
    {
        LOGD("[GWJ] %s: start. column_name[%s]. Error!!",
             __FUNCTION__, value);

        return -1;
    }
}

// 根据总数量调整  [_start_pos, _unit_num]
int MysqlTable::modify_query_boundary(int total_count)
{
    LOGD("[GWJ] %s:  start. start[%d], len[%d], total[%d]",
         __FUNCTION__, this->_start_pos, this->_unit_num, total_count);

    if(total_count > 0 && (uint32_t)total_count <= this->_start_pos)
    {
        this->_start_pos = ((total_count - 1) / this->_unit_num) * this->_unit_num;
    }

    LOGD("[GWJ] %s:  end. start[%d], len[%d], total[%d]",
         __FUNCTION__, this->_start_pos, this->_unit_num, total_count);

    return 0;
}

// 获取不分页时的记录总数
int MysqlTable::get_result_total_count(DuokooMysql& mysql)
{
    string query;
    bool is_ok = this->get_whole_count_query(query);

    if(is_ok)
    {
        int count = mysql.query(query);

        if(count < 0)
        {
            LOGD("[GWJ] %s: Get Count Error[%s]",
                    __FUNCTION__, query.c_str());

            return -1;
        }
        else if(count == 0)
        {
            LOGD("[GWJ] %s: No Records[%s]",
                    __FUNCTION__, query.c_str());

            return 0;
        }

        string total_count = mysql.getResult(0, 0);
        int total = atoi(total_count.c_str());

        LOGD("[GWJ] %s: get total[%d]", __FUNCTION__, total);
        return total;
    }
    else
    {
        LOGD("[GWJ] %s: Error query[%s]",
             __FUNCTION__, query.c_str());

        return -1;
    }
}

void MysqlTable::add_join_table(MysqlTable* join_table)
{
    this->joint_tables.push_back(join_table);
}

/*
 *  只是获取 SQL 语句中的  SELECT 段
 *  即要选出的所有列名
 */
bool MysqlTable::get_select_query(string& query)
{
    query = "SELECT ";
    int size = this->select_fields.size();

    if(size == 0)
    {
        return false;
    }

    for(int i = 0; i < size; i++)
    {
        if(i == 0)
        {
            query = query + string(this->select_fields[i]);
        }
        else
        {
            query = query + ", " + string(this->select_fields[i]);
        }
    }

    return true;
}

/*
 *  [] 运算符重载
 *  返回 ExMysqlColumn 类型的列，应该永远不能返回  NULL 指针
 *  如果传入的 column_name 不存在，则新建列，并保存在  extend_columns 中
 */
ExMysqlColumn* MysqlTable::operator[] (const char* column_name)
{
    int size = this->extend_columns.size();

    LOGD("[GWJ] %s:  start. ex_column size[%d]", __FUNCTION__, size);

    for(int i = 0; i < size; i++)
    {
        if(this->extend_columns[i].is_column_name(column_name))
        {
            LOGD("[GWJ] %s: found ex-column[%d:%s]",
                 __FUNCTION__, i, column_name);

            return &this->extend_columns[i];
        }
    }

    ExMysqlColumn new_ext_column(this, column_name);
    this->extend_columns.push_back(new_ext_column);

    ExMysqlColumn* ret = &this->extend_columns[size];
    LOGD("[GWJ] %s: Not found and creat new ex-column[%s]", __FUNCTION__, ret->column_name.c_str());
    return ret;
}

/*
 *  [] 运算符重载
 *  返回 string 类型的列, 没有时返回 NULL
 *  即设置的参与 SQL 语句中 select 段的列名
 */
const char* MysqlTable::operator[] (int i)
{
    int size = this->select_fields.size();

    if(i >= size || i < 0)
    {
        return NULL;
    }
    else
    {
        return this->select_fields[i].c_str();
    }
}

void MysqlTable::fill_mysql_result_bucket(MySQLRecord& record)
{
    int index = CommonInterface::find_key_index(record.record[this->key_name].c_str(), this->id_s);

    LOGD("[GWJ] %s: current k-v[%s:%s], index[%d]",
         __FUNCTION__, this->key_name, record.record[this->key_name].c_str(), index);

    if(index < 0)
    {
        LOGD("[GWJ] %s: 1st Record of key[%s]", __FUNCTION__, record.record[this->key_name].c_str());
        this->id_s.push_back(record.record[this->key_name].c_str());
        this->result_set[record.record[this->key_name].c_str()].push_back(record);
    }
    else
    {
        LOGD("[GWJ] %s: Exist Record of key[%d:%s]. list_id[size:%hu]",
             __FUNCTION__, index, this->id_s[index], this->id_s.size());

        this->result_set[this->id_s[index]].push_back(record);
    }
}

void MysqlTable::fill_result_set(DuokooMysql& mysql, int record_count)
{
    int size = this->select_fields.size();

    LOGD("[GWJ] %s:  start. row[%d], col[%d]", __FUNCTION__, record_count, size);

    if(!IS_VALUED_STRING(this->key_name))
    {
        this->key_name = this->select_fields[0].c_str();
        LOGD("[GWJ] %s: NULL KEY_NAME settled. Use default KEY_NAME[%s]",
             __FUNCTION__, this->key_name);
    }

    for(int i = 0; i < record_count; ++i)
    {
        MySQLRecord record;

        for(int j = 0; j < size; j++)
        {
            record.record[this->select_fields[j].c_str()] = mysql.getResult(i, j);
        }

        this->fill_mysql_result_bucket(record);
    }

    LOGD("[GWJ] %s:  end", __FUNCTION__);
}

/*
 *  设置最终 SQL 语句中 select 段的列名
 */
void MysqlTable::add_select_column(const char* col, const char* nick_name)
{
    string column_name;

    bool is_nik = this->is_had_nick_name(col);

    if(is_nik)
    {
        LOGD("[GWJ] %s: Already nicked name[%s]", __FUNCTION__, col);
        column_name = col;
    }
    else
    {
        if(IS_VALUED_STRING(nick_name))
        {
            column_name = string(nick_name) + "." + col;
            LOGD("[GWJ] %s: New nick name[%s]", __FUNCTION__, column_name.c_str());
        }
        else
        {
            column_name = col;
        }
    }

    LOGD("[GWJ] %s: push column_name[%s]", __FUNCTION__, column_name.c_str());
    this->select_fields.push_back(column_name);
}

/*
 *  设置最终 SQL 语句中 select 段的列名
 */
void MysqlTable::add_select_column(const char* col)
{
    string column_name;

    bool is_nik = this->is_had_nick_name(col);

    if(is_nik)
    {
        LOGD("[GWJ] %s: Already nicked name[%s]", __FUNCTION__, col);
        column_name = col;
    }
    else
    {
        if(IS_VALUED_STRING(this->nick_name))
        {
            column_name = string(this->nick_name) + ".`" + col +"`";
            LOGD("[GWJ] %s: New nick name[%s]", __FUNCTION__, column_name.c_str());
        }
        else
        {
            column_name = string("`") + col + "`";
        }
    }

    LOGD("[GWJ] %s: push column_name[%s]", __FUNCTION__, column_name.c_str());
    this->select_fields.push_back(column_name);
}

/*
 *  获取当前主表的 WHERE 语句
 *
 *  【注意】
 *  在处理  AND / OR 逻辑时，如果有 OR 逻辑的话，两端的判断语句都会增加一个 ( )
 *  例： (xxx) OR (xxx) 无论 xxx 的具体内容是什么
 */
bool MysqlTable::get_self_where_query(string& where_query)
{
    int  size = this->extend_columns.size();
    int  valid_index = 0;
    bool has_where = false;
    bool has_or_logic = false;

    LOGD("[GWJ] %s: Ex-Columns size[%d]", __FUNCTION__, size);

    for(int i = 0; i < size; i++)
    {
        // 连接不同 Ex-column 之间逻辑的 and/or 语句
        const char* logic_binder;

        if(valid_index > 0)
        {
            logic_binder = this->extend_columns[i].get_header_where_type();

            if(!IS_VALUED_STRING(logic_binder))
            {
                LOGD("[GWJ] %s: logic_binder NULL Error!! column[%s]",
                        __FUNCTION__, this->extend_columns[i].column_name.c_str());

                break;
            }
        }

        string cur_condition;
        has_where = this->extend_columns[i].get_where_condition_query(cur_condition);

        if(has_where == true)
        {
            // 如果有 WHERE 语句
            if(valid_index > 0)
            {
                /*
                 *   同一张表内连接多个 Ex-column 之间的判断条件
                 *   如果遇到 OR 逻辑，则需要先将前面的逻辑括起来
                 */
                if(strcmp(logic_binder, "OR") == 0)
                {
                    if(has_or_logic == false)
                    {
                        /*
                         *  之前还没有 OR 逻辑，先将前面的逻辑括起来
                         *  并先给后面的逻辑一个前括号
                         */
                        where_query = "(" + where_query + ")";
                        cur_condition = string(logic_binder) + "(" + cur_condition;
                        has_or_logic = true;
                    }
                    else
                    {
                        /*
                         *  之前已经有一个前括号的 OR 逻辑后半句
                         *  又来了一个 OR 逻辑
                         *
                         *  则此时先闭合之前 OR 逻辑的前括号
                         *  然后再打开一个新的
                         */
                        where_query = where_query + ")";
                        cur_condition = string(logic_binder) + "(" + cur_condition;
                    }
                }
                else
                {
                    // AND 逻辑正常连接即可
                    cur_condition = string(logic_binder) +  cur_condition;
                }
            }

            where_query = where_query + cur_condition;

            // 有一个合理的 WHERE 语句了
            valid_index++;
        }
    }

    if(valid_index == 0)
    {
        LOGD("[GWJ] %s: No WHERE table[%s].", __FUNCTION__, this->table_name);
        return false;
    }
    else
    {
        if(has_or_logic)
        {
            // 闭合最后一个 OR 逻辑的闭括号
            where_query = where_query + ")";
        }

        where_query = " (" + where_query + ") ";

        LOGD("[GWJ] %s: WHERE SQL[%s] in Table [%s]",
             __FUNCTION__, where_query.c_str(), this->table_name);

        return true;
    }
}

/*
 *  获取表中的 WHERE 语句
 *  如果有 JOIN 联合表格，则 JOIN 表的 WHERE 语句也在外层
 *  不在 JOIN 语句中，即先进行 JOIN 然后对 JOIN 后的结果进行 WHERE
 */
bool MysqlTable::get_where_query(string& where_query)
{
    int size = this->joint_tables.size();
    int valid_index = 0;
    bool has_where = false;

    LOGD("[GWJ] %s: JoinTables size[%d]", __FUNCTION__, size);

    // 先处理 JOIN 联合表中的  WHERE 条件
    for(int i = 0; i < size; i++)
    {
        string cur_where;
        has_where = this->joint_tables[i]->get_where_query(cur_where);

        if(has_where)
        {
            if(valid_index > 0)
            {
                /*
                 *  按照正常逻辑
                 *  不同表之间的判断条件应是 AND 逻辑
                 */
                cur_where = " AND " + cur_where;
            }

            where_query = where_query + cur_where;
            valid_index++;
        }
    }

    string self_where;
    has_where = this->get_self_where_query(self_where);

    if(has_where == true)
    {
        if(valid_index > 0)
        {
            where_query = where_query + " AND " + self_where;
        }
        else
        {
            where_query = self_where;
        }
    }

    if(valid_index == 0 && has_where == false)
    {
        LOGD("[GWJ] %s: No WHERE table[%s].", __FUNCTION__, this->table_name);
        return false;
    }
    else
    {
        LOGD("[GWJ] %s: WHERE SQL[%s] in Table [%s]",
             __FUNCTION__, where_query.c_str(), this->table_name);

        return true;
    }

    LOGD("[GWJ] %s:  end", __FUNCTION__);
}


bool MysqlTable::get_self_join_query(string& join_query)
{
    bool has_join = false;
    bool cur_has_join = false;
    int join_cnt = 0;

    int size = this->extend_columns.size();

    LOGD("[GWJ] %s: Self Table Ex-Columns size[%d]", __FUNCTION__, size);

    for(int i = 0; i < size; i++)
    {
        string cur_join;
        cur_has_join = this->extend_columns[i].get_join_query(cur_join);

        if(cur_has_join)
        {
            join_query = join_query + cur_join + " ";
            join_cnt++;
        }
    }

    LOGD("[GWJ] %s: JOIN count[%d]", __FUNCTION__, join_cnt);

    if(join_cnt > 0)
    {
        has_join = true;
    }

    return has_join;
}

bool MysqlTable::get_join_query(string& join_query)
{

    int valid_index = 0;
    bool has_join = false;

    // 先处理主表中的 JOIN 处理
    string self_join;
    bool self_has_join = false;
    self_has_join = this->get_self_join_query(self_join);

    if(self_has_join == true)
    {
        join_query = join_query + self_join;
        valid_index++;
    }

    // 再处理 JOIN 表中的  JOIN 条件
    int size = this->joint_tables.size();
    LOGD("[GWJ] %s: JoinTables size[%d]", __FUNCTION__, size);

    for(int i = 0; i < size; i++)
    {
        string cur_join;
        has_join = this->joint_tables[i]->get_join_query(cur_join);

        if(has_join)
        {
            join_query = join_query + cur_join;
            valid_index++;
        }
    }

    if(valid_index == 0)
    {
        LOGD("[GWJ] %s: No JOIN table[%s].", __FUNCTION__, this->table_name);
        return false;
    }
    else
    {
        LOGD("[GWJ] %s: JOIN SQL[%s] in Table [%s]",
                __FUNCTION__, join_query.c_str(), this->table_name);

        return true;
    }

    LOGD("[GWJ] %s:  end", __FUNCTION__);
}

/*
 *  获取表中的 ORDER BY 语句
 *  如果有 JOIN 联合表格，则 JOIN 表的 ORDER BY 语句在外层
 *  不在 JOIN 语句中，即先进行 JOIN 然后对 JOIN 后的结果进行 ORDER BY
 */
bool MysqlTable::get_orderby_query(string& orderby_query)
{
    int size = this->extend_columns.size();
    int valid_index = 0;
    bool has_orderby = false;

    // 先处理主表中的  ORDER BY 条件
    for(int i = 0; i < size; i++)
    {
        string cur_order;
        has_orderby = this->extend_columns[i].get_order_by_query(cur_order);

        if(has_orderby)
        {
            if(valid_index > 0)
            {
                cur_order = " , " + cur_order;
            }

            orderby_query = orderby_query + cur_order;
            valid_index++;
        }
    }

    // 再处理 JOIN 联合表中的  ORDER BY 条件
    size = this->joint_tables.size();

    for(int i = 0; i < size; i++)
    {
        string cur_order;
        has_orderby = this->joint_tables[i]->get_orderby_query(cur_order);

        if(has_orderby)
        {
            if(valid_index > 0)
            {
                cur_order = " , " + cur_order;
            }

            orderby_query = orderby_query + cur_order;
            valid_index++;
        }
    }

    if(valid_index == 0)
    {
        LOGD("[GWJ] %s: No ORDER BY.", __FUNCTION__);
        return false;
    }
    else
    {
        LOGD("[GWJ] %s: ORDER BY SQL[%s]",
             __FUNCTION__, orderby_query.c_str());

        return true;
    }
}

/*
 *  获取表中的 GROUP BY 语句
 *  如果有 JOIN 联合表格，则 JOIN 表的 GROUP BY 语句在外层
 *  不在 JOIN 语句中，即先进行 JOIN 然后对 JOIN 后的结果进行 GROUP BY
 */
bool MysqlTable::get_groupby_query(string& groupby_query)
{
    int size = this->extend_columns.size();
    int valid_index = 0;
    bool has_groupby = false;

    // 先处理主表中的  ORDER BY 条件
    for(int i = 0; i < size; i++)
    {
        string cur_groupby;
        has_groupby = this->extend_columns[i].get_group_by_query(cur_groupby);

        if(has_groupby)
        {
            if(valid_index > 0)
            {
                cur_groupby = " AND " + cur_groupby;
            }

            groupby_query = groupby_query + cur_groupby;
            valid_index++;
        }
    }

    // 再处理 JOIN 联合表中的  ORDER BY 条件
    size = this->joint_tables.size();

    for(int i = 0; i < size; i++)
    {
        string cur_groupby;
        has_groupby = this->joint_tables[i]->get_groupby_query(cur_groupby);

        if(has_groupby)
        {
            if(valid_index > 0)
            {
                cur_groupby = " AND " + cur_groupby;
            }

            groupby_query = groupby_query + cur_groupby;
            valid_index++;
        }
    }

    if(valid_index == 0)
    {
        LOGD("[GWJ] %s: No GROUP BY.", __FUNCTION__);
        return false;
    }
    else
    {
        LOGD("[GWJ] %s: GROUP BY SQL[%s]",
             __FUNCTION__, groupby_query.c_str());

        return true;
    }
}

/*
 *  获取表中的 LIMITED 语句
 *  设置分页数据
 */
bool MysqlTable::get_limited_query(string& query)
{
    char start_pos[10];
    char unit_nums[10];

    memset(start_pos, 0x00, sizeof(start_pos));
    memset(unit_nums, 0x00, sizeof(unit_nums));

    this->modify_query_boundary(this->total_results);

    sprintf(start_pos, "%d", this->_start_pos);
    sprintf(unit_nums, "%d", this->_unit_num);

    query = string(" LIMIT ") + start_pos + " , " + unit_nums;

    LOGD("[GWJ] %s: limited_query[%s]",
         __FUNCTION__, query.c_str());

    return true;
}

/*
 *  拼接  SQL 查询语句：
 *
 *  此处只将各个 SQL 语句元素组合好即可
 */
void MysqlTable::get_every_sql_elements(bool need_seperate_page)
{
    LOGD("[GWJ] %s:  start. table[%s]", __FUNCTION__, this->table_name);

    this->from_query = " FROM ";
    this->groupby_query = " GROUP BY ";
    this->orderby_query = " ORDER BY ";
    this->limited_query = " LIMITED ";

    if(!IS_VALUED_STRING(this->table_name))
    {
        LOGD("[GWJ] %s: Empty table name Error!", __FUNCTION__);
        return;
    }

    this->has_select_column = this->get_select_query(this->select_query);

    if(this->has_select_column == false)
    {
        LOGD("[GWJ] %s: Empty selected column name Error!", __FUNCTION__);
        return;
    }
    else
    {
        LOGD("[GWJ] %s: selected column[%s]",
             __FUNCTION__, this->select_query.c_str());
    }

    // 处理 FROM 语句
    if(IS_VALUED_STRING(this->nick_name))
    {
        this->from_query = this->from_query +
                           this->table_name + " AS " + this->nick_name + " ";
    }
    else
    {
        this->from_query  = this->from_query + this->table_name;
    }

    // 处理 WHERE 语句
    this->has_where = this->get_where_query(this->where_query);

    // 处理 ORDER BY 语句
    this->has_orderby = this->get_orderby_query(this->orderby_query);

    // 处理 GROUP BY 语句
    this->has_groupby = this->get_groupby_query(this->groupby_query);

    // 处理 LIMITED 语句
    if(need_seperate_page == true)
    {
        this->has_limited = this->get_limited_query(this->limited_query);
    }
    else
    {
        this->has_limited = false;
    }

    /*
     *  处理 JOIN 语句
     *  先递归处理 joint_table 中的 JOIN 语句
     *  最后处理主表中的 JOIN 语句
     */
    this->has_join = this->get_join_query(this->join_query);
}

/*
 *  拼接  SQL 获取查询个数语句：
 */
bool MysqlTable::get_whole_count_query(string& query)
{
    query = "SELECT count(*) " + this->from_query;

    LOGD("[GWJ] %s: basic_query[%s]",
            __FUNCTION__, query.c_str());

    if(this->has_join == true)
    {
        query = query + "\n" + this->join_query;

        LOGD("[GWJ] %s: join_query[%s]",
             __FUNCTION__, this->join_query.c_str());
    }

    if(this->has_where == true)
    {
        query = query + " WHERE " + this->where_query;

        LOGD("[GWJ] %s: where_query[%s]",
                __FUNCTION__, this->where_query.c_str());
    }

    LOGD("[GWJ] %s:  end[%s]", __FUNCTION__, query.c_str());
    return true;
}

/*
 *  拼接  SQL 查询语句：
 */
bool MysqlTable::get_whole_query_string(string& query)
{
    // 拼接最终的  SQL 语句
    query = this->select_query + "\n" + from_query;

    LOGD("[GWJ] %s: basic_query[%s]",
            __FUNCTION__, query.c_str());

    if(this->has_join == true)
    {
        query = query + "\n" + this->join_query;

        LOGD("[GWJ] %s: join_query[%s]",
             __FUNCTION__, this->join_query.c_str());
    }
    else
    {
        LOGD("[GWJ] %s: No JOIN", __FUNCTION__);
    }

    if(this->has_where == true)
    {
        query = query + " WHERE " + this->where_query;

        LOGD("[GWJ] %s: where_query[%s]",
                __FUNCTION__, this->where_query.c_str());
    }
    else
    {
        LOGD("[GWJ] %s: No WHERE", __FUNCTION__);
    }

    if(this->has_orderby == true)
    {
        query = query + "\n" + this->orderby_query;

        LOGD("[GWJ] %s: orderby_query[%s]",
                __FUNCTION__, this->orderby_query.c_str());
    }
    else
    {
        LOGD("[GWJ] %s: No ORDER BY", __FUNCTION__);
    }

    if(this->has_groupby == true)
    {
        query = query + "\n" + this->groupby_query;

        LOGD("[GWJ] %s: groupby_query[%s]",
                __FUNCTION__, this->groupby_query.c_str());
    }
    else
    {
        LOGD("[GWJ] %s: No GROUP BY", __FUNCTION__);
    }

    if(this->has_limited == true)
    {
        query = query + "\n" + this->limited_query;

        LOGD("[GWJ] %s: limited_query[%s]",
                __FUNCTION__, this->limited_query.c_str());
    }
    else
    {
        LOGD("[GWJ] %s: No LIMIT", __FUNCTION__);
    }

    LOGD("[GWJ] %s:  end[%s]", __FUNCTION__, query.c_str());
    return true;
}

/*
 *  拼接  SQL 插入语句：
 */
bool MysqlTable::get_whole_insert_string(string& query)
{
    query = string("INSERT INTO ") + this->table_name;
    LOGD("[GWJ] %s: start. Suffix[%s]", __FUNCTION__, query.c_str());

    string columns_list = "";
    string values_list = "";

    bool is_ok = this->append_column_values_list(columns_list, values_list);

    if(is_ok)
    {
        query = query + "(" + columns_list + ") VALUES (" + values_list + ")";
        LOGD("[GWJ] %s: Insert[%s]", __FUNCTION__, query.c_str());
        return true;
    }
    else
    {
        LOGD("[GWJ] %s: No conditions Return", __FUNCTION__);
        return false;
    }
}

/*
 *  拼接  SQL 修改语句：
 */
bool MysqlTable::get_whole_update_string(string& query)
{

    query = string("UPDATE ") + this->table_name + " ";
    LOGD("[GWJ] %s: start. Suffix[%s]", __FUNCTION__, query.c_str());

    string set_list = " SET ";

    bool is_ok = this->append_column_set_list(set_list);

    if(is_ok)
    {
        query = query + set_list;

        string where_query = "";
        is_ok = this->get_where_query(where_query);

        if(is_ok)
        {
            query = query + " WHERE " + where_query;
        }

        LOGD("[GWJ] %s: Update[%s]", __FUNCTION__, query.c_str());
        return true;
    }
    else
    {
        LOGD("[GWJ] %s: No conditions Return", __FUNCTION__);
        return false;
    }
}

/*
 *  从 this->insert_update_conditions 中取出数据
 *  并拼接这些数据形成 SQL 插入语句
 */
bool MysqlTable::append_column_values_list(string& columns, string& values)
{
    int size = this->insert_update_conditions.size();

    LOGD("[GWJ] %s: start. condition size[%d]", __FUNCTION__, size);

    if(size <=0)
    {
        LOGD("[GWJ] %s: No condition Return", __FUNCTION__);
        return false;
    }

    for(int i = 0; i < size; i++)
    {
        const char* key = this->insert_update_conditions[i].key.c_str();
        const char* p_key = CommonInterface::find_pure_column_name(key);

        if(IS_VALUED_STRING(p_key))
        {
            LOGD("[GWJ] %s: column[%s], pure_col[%s]. Use[%s]", __FUNCTION__, key, p_key, p_key);
            key = p_key;
        }

        const char* value = this->insert_update_conditions[i].value.c_str();
        bool is_str_value = this->insert_update_conditions[i].is_string_value;

        string cur_key = "";
        string cur_value = "";

        if(IS_VALUED_STRING(key))
        {
            LOGD("[GWJ] %s: Condition [%s:%s]", __FUNCTION__, key, value);

            if(i == 0)
            {
                cur_key = key;

                if(IS_VALUED_STRING(value))
                {
                    if(is_str_value == true)
                    {
                        cur_value = string("\"") + value + "\"";
                    }
                    else
                    {
                        cur_value = string(" ") + value;
                    }
                }
                else
                {
                    cur_value = string(" NULL ");
                }
            }
            else
            {
                cur_key = string(" , ") + key;

                if(IS_VALUED_STRING(value))
                {
                    if(is_str_value == true)
                    {
                        cur_value = string(" , \"") + value + "\"";
                    }
                    else
                    {
                        cur_value = string(" , ") + value;
                    }
                }
                else
                {
                    cur_value = string(", NULL ");
                }
            }

            columns = columns + cur_key;
            values = values + cur_value;
        }
        else
        {
            LOGD("[GWJ] %s: %dth condition Empty [KEY] Warnning![%s:%s]",
                 __FUNCTION__, i, key, value);
        }
    }

    LOGD("[GWJ] %s: Condition Finished", __FUNCTION__);
    return true;
}

/*
 *  从 this->insert_update_conditions 中取出数据
 *  并拼接这些数据形成 SQL 修改语句
 */
bool MysqlTable::append_column_set_list(string& set_query)
{
    int size = this->insert_update_conditions.size();

    LOGD("[GWJ] %s: start. condition size[%d]", __FUNCTION__, size);

    if(size <=0)
    {
        LOGD("[GWJ] %s: No condition Return", __FUNCTION__);
        return false;
    }

    for(int i = 0; i < size; i++)
    {
        const char* key = this->insert_update_conditions[i].key.c_str();
        const char* p_key = CommonInterface::find_pure_column_name(key);

        if(IS_VALUED_STRING(p_key))
        {
            LOGD("[GWJ] %s: column[%s], pure_col[%s]. Use[%s]", __FUNCTION__, key, p_key, p_key);
            key = p_key;
        }

        const char* value = this->insert_update_conditions[i].value.c_str();
        bool is_str_value = this->insert_update_conditions[i].is_string_value;

        string cur_key = "";
        string cur_value = "";

        if(IS_VALUED_STRING(key))
        {
            LOGD("[GWJ] %s: Condition [%s:%s]", __FUNCTION__, key, value);
            cur_key = string(this->table_name) + "." + key;

            if(i > 0)
            {
                cur_key = string(" , ") + cur_key;
            }

            if(IS_VALUED_STRING(value))
            {
                if(is_str_value == true)
                {
                    cur_value = cur_key + string("=\"") + value + "\"";
                }
                else
                {
                    cur_value = cur_key + string("=") + value;
                }
            }
            else
            {
                cur_value = cur_key + string("=0");
            }

            set_query = set_query + cur_value;
        }
        else
        {
            LOGD("[GWJ] %s: %dth condition Empty [KEY] Warnning![%s:%s]",
                 __FUNCTION__, i, key, value);
        }
    }

    LOGD("[GWJ] %s: Condition Finished", __FUNCTION__);
    return true;
}

/*
 *  返回结果集中数据的接口：
 *  key_index    第几个索引值
 *  vec_index    该索引值下的第几条数据
 *  column_index 该条数据的第几列
 */
const char* MysqlTable::get_result_set_value(int key_index,
                                             int vec_index,
                                             int column_index)
{
    int size = this->id_s.size();
    int col_size = this->select_fields.size();

    LOGD("[GWJ] %s: start. key_size[%d], column_size[%d]",
         __FUNCTION__, size, col_size);

    if(size == 0 || col_size == 0)
    {
        LOGD("[GWJ] %s: No Key Index[%d] Or No Column[%d]",
             __FUNCTION__, size, col_size);

        return "";
    }

    if(key_index < 0 || size <= key_index)
    {
        LOGD("[GWJ] %s: key_index[%d] out of bound[%d]",
             __FUNCTION__, key_index, size);

        return "";
    }

    if(column_index < 0 || col_size <= column_index)
    {
        LOGD("[GWJ] %s: column_index[%d] out of bound[%d]",
             __FUNCTION__, column_index, col_size);

        return "";
    }

    const char* key_value = this->id_s[key_index];
    LOGD("[GWJ] %s: key_value[%s]", __FUNCTION__, key_value);

    int vec_size = this->result_set[key_value].size();

    if(vec_size == 0)
    {
        LOGD("[GWJ] %s: No Vector of key[%s]",
             __FUNCTION__, key_value);

        return "";
    }

    if(vec_index < 0 || vec_size <= vec_index)
    {
        LOGD("[GWJ] %s: vec_index[%d] out of bound[%d]",
             __FUNCTION__, vec_index, vec_size);

        return "";
    }

    const char* column = this->select_fields[column_index].c_str();
    LOGD("[GWJ] %s: column[%s]", __FUNCTION__, column);

    return this->result_set[key_value][vec_index].record[column].c_str();
}

/*
 *  返回结果集中数据的接口：
 *  key       索引值
 *  vec_index 该索引值下的第几条数据
 *  column    该条数据的哪列
 *
 *  【注意】
 *  如果有 JOIN 表格并且匿名，在获取数据时
 *  column 必须以 nick_name.column_name 的形式传入才行
 */
const char* MysqlTable::get_result_set_value(const char* key,
                                             int vec_index,
                                             const char* column)
{
    int id_index = CommonInterface::find_key_index(key, this->id_s);

    if(id_index < 0)
    {
        LOGD("[GWJ] %s: No Key_Index[%s]", __FUNCTION__, key);
        return "";
    }

    string r_col;
    int col_index = -1;
    int ret = this->change_to_table_nick_column(column, r_col);

    if(ret < 0)
    {
        return "";
    }

    col_index = CommonInterface::find_key_index(r_col.c_str(), this->select_fields);

    if(col_index < 0)
    {
        LOGD("[GWJ] %s: No Column_Index[%s]", __FUNCTION__, column);
        return "";
    }

    return this->get_result_set_value(id_index, vec_index, col_index);
}

/*
 *  返回结果集中数据的接口：
 *  key       索引值
 *  vec_index 该索引值下的第几条数据
 *  column    该条数据的哪列
 *
 *  【注意】
 *  如果有 JOIN 表格并且匿名，在获取数据时
 *  column 必须以 nick_name.column_name 的形式传入才行
 */
const char* MysqlTable::get_result_set_value(int key_index,
                                             int vec_index,
                                             const char* column)
{
    string r_col;
    int col_index = 0;
    int ret = this->change_to_table_nick_column(column, r_col);

    if(ret < 0)
    {
        return "";
    }

    col_index = CommonInterface::find_key_index(r_col.c_str(), this->select_fields);

    if(col_index < 0)
    {
        LOGD("[GWJ] %s: No Column_Index[%s]", __FUNCTION__, column);
        return "";
    }

    return this->get_result_set_value(key_index, vec_index, col_index);
}

unsigned int MysqlTable::get_total_key_count()
{
    return this->id_s.size();
}

int MysqlTable::get_total_columns_count()
{
    return this->select_fields.size();
}

int MysqlTable::get_total_count_without_page()
{
    return this->total_results;
}

int MysqlTable::get_record_count_by_key_index(int key_index)
{
	int size = this->id_s.size();

	LOGD("[GWJ] %s: start. key_size[%d]", __FUNCTION__, size);

	if(size == 0)
	{
		LOGD("[GWJ] %s: No Key Index[%d]", __FUNCTION__, size);
		return -1;
	}

	if(key_index < 0 || size <= key_index)
	{
		LOGD("[GWJ] %s: key_index[%d] out of bound[%d]",__FUNCTION__, key_index, size);
		return -1;
	}

	const char* key_value = this->id_s[key_index];
	LOGD("[GWJ] %s: key_value[%s]", __FUNCTION__, key_value);

	return this->result_set[key_value].size();
}

bool MysqlTable::process_query_sql(const char* db_name, bool need_seperate_page)
{
    DuokooMysql mysql(db_name);

    // 先设定各个 SQL 元素
    this->get_every_sql_elements(need_seperate_page);

    this->total_results = this->get_result_total_count(mysql);

    if(this->total_results == 0)
    {
        LOGD("[GWJ] %s: total_results is 0. No Records", __FUNCTION__);
        return false;
    }
    else if(this->total_results < 0)
    {
        this->total_results = 0;

        LOGD("[GWJ] %s: Get total_results Error!!", __FUNCTION__);
        return false;
    }

    string query;
    bool is_ok = this->get_whole_query_string(query);

    if(is_ok)
    {
        int count = mysql.query(query);

        if(count < 0)
        {
            LOGD("[GWJ] %s: Error[%s]",
                 __FUNCTION__, query.c_str());

            return false;
        }
        else if(count == 0)
        {
            LOGD("[GWJ] %s: No Records[%s]",
                 __FUNCTION__, query.c_str());

            return false;
        }

        this->fill_result_set(mysql, count);
        return true;
    }
    else
    {
        LOGD("[GWJ] %s: SQL Error!", __FUNCTION__);
        return false;
    }
}

/*
 *  将插入操作加入事务
 */
bool MysqlTable::add_new_insert_transaction()
{
    string query;
    bool is_ok = this->get_whole_insert_string(query);
    this->insert_update_conditions.clear();

    if(is_ok == true)
    {
        LOGD("[GWJ] %s: add[%s] to transaction", __FUNCTION__, query.c_str());
        this->insert_transaction_sqls.push_back(query);
        return true;
    }
    else
    {
        LOGD("[GWJ] %s: No INSERT SQL for transaction", __FUNCTION__);
        return false;
    }
}

/*
 *  将修改操作加入事务
 */
bool MysqlTable::add_new_update_transaction()
{
    string query;
    bool is_ok = this->get_whole_update_string(query);

    if(is_ok == true)
    {
        LOGD("[GWJ] %s: add[%s] to transaction", __FUNCTION__, query.c_str());
        this->update_transaction_sqls.push_back(query);
        return true;
    }
    else
    {
        LOGD("[GWJ] %s: No UPDATE SQL for transaction", __FUNCTION__);
        return false;
    }
}

/*
 *  执行插入语句，向数据库中新增数据
 */
bool MysqlTable::process_insert_transaction(const char* db_name)
{
    int size = this->insert_transaction_sqls.size();

    if(size == 0)
    {
        LOGD("[GWJ] %s: No INSERT SQL in Transaction", __FUNCTION__);
        return false;
    }
    else
    {
        LOGD("[GWJ] %s: %d INSERT SQLs in Transaction to Process",
             __FUNCTION__, size);

        DuokooMysql mysql(db_name);
        int count = mysql.queryInTransaction(this->insert_transaction_sqls);

        if(count < 0)
        {
            LOGD("[GWJ] %s: Error", __FUNCTION__);

            // 此处是否需要回滚？
//            mysql.rollback();
            return false;
        }
        else
        {
            LOGD("[GWJ] %s: OK", __FUNCTION__);
            return true;
        }
    }
}

/*
 *  执行修改语句，向数据库中新增数据
 */
bool MysqlTable::process_update_transaction(const char* db_name)
{
    int size = this->update_transaction_sqls.size();

    if(size == 0)
    {
        LOGD("[GWJ] %s: No UPDATE SQL in Transaction", __FUNCTION__);
        return false;
    }
    else
    {
        LOGD("[GWJ] %s: %d UPDATE SQLs in Transaction to Process",
             __FUNCTION__, size);

        DuokooMysql mysql(db_name);
        int count = mysql.queryInTransaction(this->update_transaction_sqls);

        if(count < 0)
        {
            LOGD("[GWJ] %s: Error", __FUNCTION__);

            // 此处是否需要回滚？
//            mysql.rollback();
            return false;
        }
        else
        {
            LOGD("[GWJ] %s: OK", __FUNCTION__);
            return true;
        }
    }
}

/*
 *  设置分页数
 */
void MysqlTable::set_unit_info(const uint32_t unit_num,
                               const uint32_t unit_times)
{
    if(unit_num == 0)
    {
        _unit_num = 10;
    }
    else
    {
        _unit_num = unit_num;
    }
    if(unit_times == 0)
    {
        _unit_times = 1;
    }
    else
    {
        _unit_times = unit_times;
    }

    _start_pos = _unit_num * (_unit_times - 1);

    LOGD("[GWJ] %s: unit_num[%d], unit_times[%d], start_pos[%d]]",
         __FUNCTION__, _unit_num, _unit_times, _start_pos);
}
