/*
 * common_simple_buf.cpp
 *
 *  Created on: 2013-5-15
 *      Author: guoweijiang
 */
#include "common_simple_buf.h"

CommonSimpleBuffer::CommonSimpleBuffer()
{
    this->m_public_propertys = new MysqlTable("mcp_content_pub_prop");
    this->m_game_info_type = new MysqlTable("mcp_content_news_type");
    this->init_buffer();
}

CommonSimpleBuffer::~CommonSimpleBuffer()
{
    delete this->m_public_propertys;
    delete this->m_game_info_type;
}

CommonSimpleBuffer& CommonSimpleBuffer::operator = (const CommonSimpleBuffer& cp_obj)
{
}

int CommonSimpleBuffer::fill_game_info_type_name()
{
    LOGD("[GWJ] %s: start", __FUNCTION__);

    this->m_game_info_type->add_select_column("id");
    this->m_game_info_type->add_select_column("name");

    // 不分页，有多少取多少
    bool ret = this->m_game_info_type->process_query_sql("default_mysql", false);

    if(ret == false)
    {
        LOGD("[GWJ] %s: Query Error!", __FUNCTION__);
        return -1;
    }
    else
    {
        LOGD("[GWJ] %s: Query OK!", __FUNCTION__);
        return 0;
    }
}

int CommonSimpleBuffer::fill_public_property()
{
    LOGD("[GWJ] %s: start", __FUNCTION__);

    this->m_public_propertys->add_select_column("id");
    this->m_public_propertys->add_select_column("prop_url");

    // 不分页，有多少取多少
    bool ret = this->m_public_propertys->process_query_sql("default_mysql", false);

    if(ret == false)
    {
        LOGD("[GWJ] %s: Query Error!", __FUNCTION__);
        return -1;
    }
    else
    {
        LOGD("[GWJ] %s: Query OK!", __FUNCTION__);
        return 0;
    }
}

int CommonSimpleBuffer::get_public_property_url(const char* key, const char* prop_url)
{
    int size = this->m_public_propertys->get_total_key_count();

    LOGD("[GWJ] %s: start. pub_prop key[%d]", __FUNCTION__, size);

    if(size == 0)
    {
        prop_url = NULL;

        LOGD("[GWJ] %s: No pub_prop_url", __FUNCTION__);
        return -1;
    }

    prop_url = this->m_public_propertys->get_result_set_value(key, 0, "prop_url");

    if(IS_VALUED_STRING(prop_url))
    {
        LOGD("[GWJ] %s: pub_prop_url[%s]", __FUNCTION__, prop_url);
        return 1;
    }
    else
    {
        LOGD("[GWJ] %s: key[%s] has no pub_prop_url",
             __FUNCTION__, key);

        return 0;
    }
}

int CommonSimpleBuffer::get_game_info_type_name(const char* id, string& type_name_value)
{
    LOGD("[GWJ] %s: start. id[%s]", __FUNCTION__, id);

    const char* type_name = this->m_game_info_type->get_result_set_value(id, 0, "name");
    type_name_value = type_name;

    LOGD("[GWJ] %s: id[%s], type_name[%s], value[%s]",
         __FUNCTION__, id, type_name, type_name_value.c_str());

    return 1;
}

int CommonSimpleBuffer::init_buffer()
{
    this->fill_public_property();
    this->fill_game_info_type_name();
    return 1;
}


