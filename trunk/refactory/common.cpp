/*
 * common.cpp
 *
 *  Created on: 2013-4-28
 *      Author: guoweijiang
 */
#include "common.h"

CommonInterface::CommonInterface()
{
}

CommonInterface::~CommonInterface()
{
}

// 找到 value 在 list 中的索引，不存在时返回 -1
int CommonInterface::find_key_index(const char* value, vector<const char*>& list_values)
{
    if(!IS_VALUED_STRING(value))
    {
        return -1;
    }

    int size = list_values.size();

    for(int i = 0; i < size; i++)
    {
        if(IS_SAME_VALUED_STRING(value, list_values[i]))
        {
            return i;
        }
    }

    return -1;
}

int CommonInterface::find_key_index(const char* value, vector<string>& list_values)
{
    if(!IS_VALUED_STRING(value))
    {
        return -1;
    }

    int size = list_values.size();

    for(int i = 0; i < size; i++)
    {
        if(IS_SAME_VALUED_STRING(value, list_values[i].c_str()))
        {
            return i;
        }
    }

    return -1;
}

int CommonInterface::find_key_index(const char* value, list<const char*>& list_values)
{
    if(!IS_VALUED_STRING(value))
    {
        return -1;
    }

    list<const char*>::iterator it = list_values.begin();
    list<const char*>::iterator end = list_values.end();

    for(int i = 0; it != end; it++, i++)
    {
        if(IS_SAME_VALUED_STRING(value, (*it)))
        {
            return i;
        }
    }

    return -1;
}

int CommonInterface::find_key_index(const char* value, list<string>& list_values)
{
    if(!IS_VALUED_STRING(value))
    {
        return -1;
    }

    list<string>::iterator it = list_values.begin();
    for(int i = 0; it != list_values.end(); it++, i++)
    {
        if(IS_SAME_VALUED_STRING(value, (*it).c_str()))
        {
            return i;
        }
    }

    return -1;
}

// 数组赋值拷贝
void CommonInterface::copy_vector_values(vector<string>& src, vector<string>& des)
{
    int size = src.size();

    for(int i = 0; i < size; i++)
    {
        des[i] = src[i];
    }
}

// 删除字符串数组数据
void CommonInterface::clear_vector_string(vector<string>& list)
{
    int size = list.size();

    for(int i = 0; i < size; i++)
    {
        list[i].clear();
    }

    list.clear();
}

// 找到 MYSQL 表中不含匿名的列名
const char* CommonInterface::find_pure_column_name(const char* column_name)
{
    int len = strlen(column_name);

    for(int i = 0; i < len; i++)
    {
        if(column_name[i] == '.')
        {
            LOGD("[GWJ] %s: pure column name[%s]",
                 __FUNCTION__, &column_name[i+1]);

            return &column_name[i+1];
        }
    }

    return NULL;
}

/*
 * 判断输入的字符串是否是标准的列名形式，即 `column_name`
 *
 * 最终结果放入 standard_column 保证其实一个标准格式
 *
 * return : 0 : 不是标准格式，需要转换
 *          1 : 已经是标准格式，不需要转换
 *          -1 : 错误的列名格式
 */
int CommonInterface::is_standard_column_format(const char* column_name, string& standard_column)
{
    LOGD("[GWJ] %s: start. column[%s]", __FUNCTION__, column_name);

    if(IS_VALUED_STRING(column_name))
    {
        int len = strlen(column_name);
        int tag_num = 0;

        for(int i = 0; i < len; i++)
        {
            if(column_name[i] == '`')
            {
                tag_num++;

                LOGD("[GWJ] %s: column name[%s] has standard tag_num[%d]",
                     __FUNCTION__, column_name, tag_num);

                if(tag_num == 2)
                {
                    standard_column = column_name;

                    LOGD("[GWJ] %s: column name[%s] Is tandard tag_num[%d]. Result[%s]",
                         __FUNCTION__, column_name, tag_num, standard_column.c_str());

                    return 1;
                }
            }
        }

        if(tag_num == 0)
        {
            standard_column = string("`") + column_name + "`";

            LOGD("[GWJ] %s: column name[%s] Not standard tag_num[%d]. Need add Tag[%s]",
                 __FUNCTION__, column_name, tag_num, standard_column.c_str());

            return 0;
        }
        else
        {
            LOGD("[GWJ] %s: column name[%s] tag_num[%d].Error Column!!",
                 __FUNCTION__, column_name, tag_num);

            return -1;
        }
    }
    else
    {
        LOGD("[GWJ] %s: Not Valued String. ERROR!!", __FUNCTION__);
        return -1;
    }
}

/*
 * 获取输入 column 中的 nick name, 保存到输出参数中
 *
 * return : 0 : column 中不含 nick_name
 *          1 : 包含 nick_name 并返回
 *          -1 : 错误的列名
 */
int CommonInterface::get_nick_name(const char* column, string& nick_name)
{
    LOGD("[GWJ] %s: start. column[%s]", __FUNCTION__, column);

    if(IS_VALUED_STRING(column))
    {
        int len = strlen(column);
        nick_name = "";

        for(int i = 0; i < len; i++)
        {
            if(column[i] != '.')
            {
                nick_name = nick_name + column[i];
            }
            else
            {
                LOGD("[GWJ] %s: Get Nick Name[%s]",
                     __FUNCTION__, nick_name.c_str());

                return 1;
            }
        }

        LOGD("[GWJ] %s: No Nick Name[%s]",
             __FUNCTION__, column);

        return 0;
    }
    else
    {
        LOGD("[GWJ] %s: Not Valued String. ERROR!!", __FUNCTION__);
        return -1;
    }
}

/*
 *  按照指定符号分割字符串
 *  return : 返回分割后的字符串个数
 */
int CommonInterface::splic_string(string& orig_string, vector<string>& split_result, const char* seperator)
{
    if(orig_string.empty())
    {
        LOGD("[GWJ] %s: Empty string", __FUNCTION__);
        return 0;
    }

    int pos = 0;

    while(!orig_string.empty())
    {
        pos = orig_string.find(seperator);
        if(pos != string::npos)
        {
            string ss = orig_string.substr(0,pos);
            split_result.push_back(ss);
            LOGD("[GWJ] %s: type is %s", __FUNCTION__, ss.c_str());

            orig_string.erase(0,pos+1);
        }
        else
        {
            split_result.push_back(orig_string);
            orig_string="";
        }
    }

    return split_result.size();
}

/*
 *  按照指定符号分割字符串
 *  return : 返回分割后的字符串个数
 */
int CommonInterface::splic_string(char* orig_string, vector<char*>& split_result, const char* seperator)
{
    if(!IS_VALUED_STRING(orig_string))
    {
        LOGD("[GWJ] %s: Empty string", __FUNCTION__);
        return 0;
    }

    int str_len = strlen(orig_string);

    int sub_header = 0;
    int i = 0;
    for(; i < str_len; i++)
    {
        if(orig_string[i] == (*seperator))
        {
            orig_string[i] = '\0';
            LOGD("[GWJ] %s: Sub string[%s]", __FUNCTION__, &orig_string[sub_header]);
            split_result.push_back(&orig_string[sub_header]);
            sub_header = i + 1;
        }
    }

    if(orig_string[i-1] != (*seperator))
    {
        LOGD("[GWJ] %s: Last Sub string[%s]", __FUNCTION__, &orig_string[sub_header]);
        split_result.push_back(&orig_string[sub_header]);
    }

    return split_result.size();
}

char* CommonInterface::elimite_shell_flag(char* orig_string,
                                          const char start_flag,
                                          const char end_flag)
{
    if(!IS_VALUED_STRING(orig_string))
    {
        LOGD("[GWJ] %s: Empty string", __FUNCTION__);
        return "";
    }
    else
    {
        int str_len = strlen(orig_string);

        LOGD("[GWJ] %s: orig_string[%s], str_len[%d], shell_flag[%c, %c]",
             __FUNCTION__, orig_string, str_len, start_flag, end_flag);

        if(orig_string[0] == start_flag && orig_string[str_len-1] == end_flag)
        {
            orig_string[0] = '\0';
            orig_string[str_len-1] = '\0';

            LOGD("[GWJ] %s: Core orig_string[%s] without Shell Flag",
                 __FUNCTION__, &orig_string[1]);

            return &orig_string[1];
        }
        else
        {
            LOGD("[GWJ] %s: orig_string[%s]. No Shell Flag Return",
                 __FUNCTION__, orig_string);

            return orig_string;
        }
    }
}

void CommonInterface::set_struct_mongodb_value(bson* b,
                                               const char* field_name,
                                               vector<const char*>& struct_cols,
                                               string& mysql_value)
{
    char index[8];

    int col_size = struct_cols.size();
    if(col_size == 0)
    {
        return;
    }

    vector<string> values;
    CommonInterface::splic_string(mysql_value, values, ",");

    bson_append_start_array(b, field_name);

    int valuse_size = values.size();
    int index_i = 0;
    for(int i = 0; i < valuse_size; i += col_size, index_i++)
    {
        memset(index, 0x00, sizeof(index));
        sprintf(index, "%d", index_i);
        bson_append_start_object(b, index);

        for(int j = 0; j < col_size; j++)
        {
            bson_append_string(b, struct_cols[j], values[i + j].c_str());
        }

        bson_append_finish_object(b);
    }

    CommonInterface::clear_vector_string(values);
    bson_append_finish_array(b);
    return;
}

void CommonInterface::set_array_mongodb_value(bson* b,
                                              const char* field_name,
                                              string& mysql_value)
{
    char index[8];
    vector<string> values;
    CommonInterface::splic_string(mysql_value, values, ",");

    bson_append_start_array(b, field_name);

    int valuse_size = values.size();
    for(int i = 0; i < valuse_size; i++)
    {
        memset(index, 0x00, sizeof(index));
        sprintf(index, "%d", i);
        bson_append_string(b, index, values[i].c_str());
    }

    CommonInterface::clear_vector_string(values);
    bson_append_finish_array(b);
    return;
}

// 获取系统当期时间戳
void CommonInterface::get_time_now(string& tm_now)
{
    time_t cur = time(NULL);
    time(&cur);

    struct tm *curtm = localtime(&cur);

    char time_str[64];
    memset(time_str, 0x00, sizeof(time_str));

    sprintf(time_str, "%d-%02d-%02d %02d:%02d:%02d",
                curtm->tm_year+1900, curtm->tm_mon+1,
                curtm->tm_mday, curtm->tm_hour,
                curtm->tm_min,  curtm->tm_sec);

    tm_now = time_str;
}


double CommonInterface::get_interface_process_time( struct timeval &s, struct timeval &e)
{
	struct timeval start;
	int interval = 1000000*(e.tv_sec - s.tv_sec) + (e.tv_usec - s.tv_usec);
	return (double)interval/1000.0;
}
pid_t CommonInterface::get_thread_id()
{
	return syscall(SYS_gettid);
}
