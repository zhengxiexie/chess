/*
 * redis_module.cpp
 *
 *  Created on: 2013-6-5
 *      Author: guoweijiang
 */

#include "redis_module.h"
#include <exception>

RedisModule::RedisModule()
{
    LOGD("[GWJ] %s: start", __FUNCTION__);
    this->conn = redisConnPool::getInstance()->getConnection();
    this->reply = NULL;
    LOGD("[GWJ] %s: end", __FUNCTION__);
}

RedisModule::~RedisModule()
{
    LOGD("[GWJ] %s: start", __FUNCTION__);

    if(this->conn != NULL)
    {
        redisConnPool::getInstance()->releaseConnection(this->conn);
        LOGD("[GWJ] %s: releaseConnection", __FUNCTION__);
    }
    else
    {
        LOGD("[GWJ] %s: Redis Connection NULL?!", __FUNCTION__);
    }

    if(this->reply != NULL)
    {
        freeReplyObject(this->reply);
        this->reply = NULL;
        LOGD("[GWJ] %s: freeReply", __FUNCTION__);
    }
    else
    {
        LOGD("[GWJ] %s: Redis Reply NULL?!", __FUNCTION__);
    }

    LOGD("[GWJ] %s: end", __FUNCTION__);
}

int RedisModule::add_new_tuple(const char* key, const char* value, int minites_to_live)
{
    if(this->conn == NULL)
    {
        LOGD("[GWJ] %s: get redis connection failed", __FUNCTION__);
        return -5;
    }
	
    this->reply = (redisReply*)redisCommand(this->conn, "SET %s %s", key, value);
	if(0!=minites_to_live)
	{
		redisCommand(this->conn, "EXPIRE %s %ld", key, minites_to_live*60);
		//FREE_REDIS_REPLY(this->reply)
	}
    try
    {
        if(this->reply != NULL)
        {
            if(IS_REDIS_REPLY_ERROR(this->reply->type))
            {
                LOGD("[GWJ] %s: REDIS ERROR", __FUNCTION__);
                return -5;
            }
            else
            {
                LOGD("[GWJ] %s: OK end", __FUNCTION__);
                return 0;
            }
        }
        else
        {
            LOGD("[GWJ] %s: get a NULL redis reply", __FUNCTION__);
            return -5;
        }
    }
    catch(exception& e)
    {
        LOGA("[GWJ] %s: Redis Exception[%s]", __FUNCTION__, e.what());
        return -5;
    }
    catch(...)
    {
        LOGA("[GWJ] %s: Redis Unknown Exception", __FUNCTION__);
        return -5;
    }
}

int RedisModule::add_new_key_value(const char* key, const char* value, int minites_to_live)
{
    if(this->conn == NULL)
    {
        LOGD("[GWJ] %s: get redis connection failed", __FUNCTION__);
        return -5;
    }
	
    this->reply = (redisReply *)redisCommand(this->conn, "SADD %s %s", key, value);
	if(0!=minites_to_live)
	{
		redisCommand(this->conn, "EXPIRE %s %ld", key, minites_to_live*60);
		//FREE_REDIS_REPLY(this->reply)
	}
    try
    {
        if(this->reply != NULL)
        {
            if(IS_REDIS_REPLY_ERROR(this->reply->type))
            {
                LOGD("[GWJ] %s: REDIS ERROR", __FUNCTION__);
                return -5;
            }
            else
            {
                LOGD("[GWJ] %s: OK end", __FUNCTION__);
                return 0;
            }
        }
        else
        {
            LOGD("[GWJ] %s: get a NULL redis reply", __FUNCTION__);
            return -5;
        }
    }
    catch(exception& e)
    {
        LOGA("[GWJ] %s: Redis Exception[%s]", __FUNCTION__, e.what());
        return -5;
    }
    catch(...)
    {
        LOGA("[GWJ] %s: Redis Unknown Exception", __FUNCTION__);
        return -5;
    }
}

int RedisModule:: push_value_list(const char* key, const char* value, int minites_to_live)
{
    if(this->conn == NULL)
    {
        LOGD("[GWJ] %s: get redis connection failed", __FUNCTION__);
        return -5;
    }
	
    this->reply = (redisReply *)redisCommand(this->conn, "RPUSH %s %s", key, value);
	if(0!=minites_to_live)
	{
		redisCommand(this->conn, "EXPIRE %s %ld", key, minites_to_live*60);
		//FREE_REDIS_REPLY(this->reply)
	}
    try
    {
        if(this->reply != NULL)
        {
            if(IS_REDIS_REPLY_ERROR(this->reply->type))
            {
                LOGD("[GWJ] %s: REDIS ERROR", __FUNCTION__);
                return -5;
            }
            else
            {
                LOGD("[GWJ] %s: OK end", __FUNCTION__);
                return 0;
            }
        }
        else
        {
            LOGD("[GWJ] %s: get a NULL redis reply", __FUNCTION__);
            return -5;
        }
    }
    catch(exception& e)
    {
        LOGA("[GWJ] %s: Redis Exception[%s]", __FUNCTION__, e.what());
        return -5;
    }
    catch(...)
    {
        LOGA("[GWJ] %s: Redis Unknown Exception", __FUNCTION__);
        return -5;
    }
}

//add by zhengxie
int RedisModule::add_new_key_record(const char* key, const map<string, string>& record, int minites_to_live)
{
    if(this->conn == NULL)
    {
        LOGD("[GWJ] %s: get redis connection failed", __FUNCTION__);
        return -5;
    }

    try
    {
        map<string, string>::const_iterator it = record.begin();
        string field("");
        string value("");

        if(0!=minites_to_live)
        {
            this->reply = (redisReply*)redisCommand(this->conn, "EXPIRE %s %ld", key, minites_to_live*60);
            FREE_REDIS_REPLY(this->reply)
        }

        for(; it != record.end(); it++)
        {
            field = it->first;
            value = it->second;

            if(!IS_VALUED_STRING(value.c_str()))
            {
                LOGA("[GWJ] %s: Record Value Null Error!", __FUNCTION__);
                return -5;
            }

            if(!IS_VALUED_STRING(field.c_str()))
            {
                LOGA("[GWJ] %s: Record Field Null Error!", __FUNCTION__);
                return -5;
            }

            this->reply = (redisReply *)redisCommand(this->conn, "HMSET %s %s %s", key,
                                                     field.c_str(), value.c_str());
            if(this->reply != NULL)
            {
                if(IS_REDIS_REPLY_ERROR(this->reply->type))
                {
                    LOGD("[GWJ] %s: REDIS ERROR", __FUNCTION__);
                    return -5;
                }
                else
                {
                    LOGD("[GWJ] %s: REDIS OK Continue Next", __FUNCTION__);
                    FREE_REDIS_REPLY(this->reply)
                }
            }
            else
            {
                LOGD("[GWJ] %s: get a NULL redis reply", __FUNCTION__);
                return -5;
            }
        }

        return 0;
    }
    catch(exception& e)
    {
        LOGA("[GWJ] %s: Redis Exception[%s]", __FUNCTION__, e.what());
        return -5;
    }
    catch(...)
    {
        LOGA("[GWJ] %s: Redis Unknown Exception", __FUNCTION__);
        return -5;
    }
}

//add by zhengxie
int RedisModule::get_record_by_key(const char* key, map<string, string>& record) 
{
    if(this->conn == NULL)
    {
        LOGD("[GWJ] %s: get redis connection failed", __FUNCTION__);
        return -5;
    }

    try
    {
        this->reply = (redisReply*)redisCommand(this->conn, "HKEYS %s", key);
        vector<string> fields;

        if(this->reply != NULL)
        {
            if(IS_REDIS_REPLY_ERROR(this->reply->type))
            {
                LOGD("[GWJ] %s: REDIS ERROR", __FUNCTION__);
                return -5;
            }
            else
            {
                int count = this->reply->elements;

                if(0 == count)
                {
                    LOGD("[GWJ] %s: NO RECORD", __FUNCTION__);
                    return -1;
                }

                LOGD("[GWJ] %s: set count is %d", __FUNCTION__, count);
                for(int i = 0; i < count; i++)
                {
                    if(IS_VALUED_STRING(this->reply->element[i]->str))
                    {
                        fields.push_back(this->reply->element[i]->str);
                    }
                    else
                    {
                        LOGA("[GWJ] %s: Redis Reply[%d]->str NULL Error!", __FUNCTION__, i);
                        return -5;
                    }
                }// for
            }// if(IS_REDIS_REPLY_ERROR(this->reply->type))
        }
        else
        {
            LOGD("[GWJ] %s: get a NULL redis reply", __FUNCTION__);
            return -5;
        }

        FREE_REDIS_REPLY(this->reply)
        vector<string>::iterator it = fields.begin();
        string value("");

        for(; it != fields.end(); ++it)
        {
            this->reply = (redisReply *)redisCommand(this->conn, "HGET %s %s", key,
                                                     it->c_str());
            if(this->reply != NULL)
            {
                if(IS_REDIS_REPLY_ERROR(this->reply->type))
                {
                    LOGD("[GWJ] %s: REDIS ERROR", __FUNCTION__);
                    return -5;
                }
                else
                {
                    //if(IS_VALUED_STRING(this->reply->str))
                    //{
                        record.insert(pair<string, string> (*it, this->reply->str));

                        if(this->reply != NULL)
                        {
                            LOGD("[GWJ] %s: REDIS Reply Free", __FUNCTION__);
                            freeReplyObject(this->reply);
                            this->reply = NULL;
                        }
                        else
                        {
                            LOGA("[GWJ] %s: REDIS ERROR. Why Reply NULL?!", __FUNCTION__);
                            return -5;
                        }
                    /*}*/
                    //else
                    //{
                        //LOGA("[GWJ] %s: REDIS ERROR. Reply_str NULL", __FUNCTION__);
                        //return -5;
                    /*}*/
                }
            }
            else
            {
                LOGD("[GWJ] %s: get a NULL redis reply", __FUNCTION__);
                return -5;
            }
        }

        return 0;
    }
    catch(exception& e)
    {
        LOGA("[GWJ] %s: Redis Exception[%s]", __FUNCTION__, e.what());
        return -5;
    }
    catch(...)
    {
        LOGA("[GWJ] %s: Redis Unknown Exception", __FUNCTION__);
        return -5;
    }
}

    
int RedisModule::del_key_value(const char* key, const char* value)
{
    if(this->conn == NULL)
    {
        LOGD("[GWJ] %s: get redis connection failed", __FUNCTION__);
        return -5;
    }

    try
    {
        this->reply = (redisReply *)redisCommand(this->conn, "SREM %s %s", key, value);

        if(this->reply != NULL)
        {
            if(IS_REDIS_REPLY_ERROR(this->reply->type))
            {
                LOGD("[GWJ] %s: REDIS ERROR", __FUNCTION__);
                return -5;
            }
            else
            {
                LOGD("[GWJ] %s: OK end", __FUNCTION__);
                return 0;
            }
        }
        else
        {
            LOGD("[GWJ] %s: get a NULL redis reply", __FUNCTION__);
            return -5;
        }
    }
    catch(exception& e)
    {
        LOGA("[GWJ] %s: Redis Exception[%s]", __FUNCTION__, e.what());
        return -5;
    }
    catch(...)
    {
        LOGA("[GWJ] %s: Redis Unknown Exception", __FUNCTION__);
        return -5;
    }
}

int RedisModule::del_key_record(const char* key)
{
    if(this->conn == NULL)
    {
        LOGD("[GWJ] %s: get redis connection failed", __FUNCTION__);
        return -5;
    }

    try
    {
        this->reply = (redisReply *)redisCommand(this->conn, "DEL %s", key);

        if(this->reply != NULL)
        {
            if(IS_REDIS_REPLY_ERROR(this->reply->type))
            {
                LOGD("[GWJ] %s: REDIS ERROR", __FUNCTION__);
                return -5;
            }
            else
            {
                LOGD("[GWJ] %s: OK end", __FUNCTION__);
                return 0;
            }
        }
        else
        {
            LOGD("[GWJ] %s: get a NULL redis reply", __FUNCTION__);
            return -5;
        }
    }
    catch(exception& e)
    {
        LOGA("[GWJ] %s: Redis Exception[%s]", __FUNCTION__, e.what());
        return -5;
    }
    catch(...)
    {
        LOGA("[GWJ] %s: Redis Unknown Exception", __FUNCTION__);
        return -5;
    }
}

int RedisModule::remove_key_value_list(const char* key, list<string>& values)
{
    int size = values.size();
    list<string>::iterator it = values.begin();
    LOGD("[GWJ] %s: value_size[%d]", __FUNCTION__, size);

    if(size == 0 || size < 0)
    {
        LOGD("[GWJ] %s: value list emtpy", __FUNCTION__);
        return 0;
    }

    if(this->conn == NULL)
    {
        LOGD("[GWJ] %s: get redis connection failed", __FUNCTION__);
        return -5;
    }

    try
    {
        for(; it != values.end(); it++)
        {
            LOGD("[GWJ] %s: remove value[%s:%s]", __FUNCTION__, key, (*it).c_str());
            this->reply = (redisReply *)redisCommand(this->conn, "SREM %s %s", key, (*it).c_str());
            FREE_REDIS_REPLY(this->reply)
        }

        return 1;
    }
    catch(exception& e)
    {
        LOGA("[GWJ] %s: Redis Exception[%s]", __FUNCTION__, e.what());
        return -5;
    }
    catch(...)
    {
        LOGA("[GWJ] %s: Redis Unknown Exception", __FUNCTION__);
        return -5;
    }
}

int RedisModule::get_rank_by_key(const char* key, const char* set_name, int order_type)
{
    if(this->conn == NULL)
    {
        LOGD("[GWJ] %s: get redis connection failed", __FUNCTION__);
        return -5;
    }

    try
    {
		if( order_type == 0 )
		{
			this->reply = (redisReply *)redisCommand(this->conn, "ZREVRANK %s %s", set_name, key);
		}
		else if( order_type == 1 )
		{
			this->reply = (redisReply *)redisCommand(this->conn, "ZRANK %s %s", set_name, key);
		}

        if(this->reply != NULL)
        {
            if(IS_REDIS_REPLY_ERROR(this->reply->type))
            {
                LOGD("[GWJ] %s: REDIS ERROR", __FUNCTION__);
                return -5;
            }
            else
            {
				int rank = this->reply->integer;
				return rank;
				LOGD("[GWJ] %s: rank[%d]", __FUNCTION__, rank);
				return 0;
            }
        }
        else
        {
            LOGD("[GWJ] %s: get a NULL redis reply", __FUNCTION__);
            return -5;
        }
    }
    catch(exception& e)
    {
        LOGA("[GWJ] %s: Redis Exception[%s]", __FUNCTION__, e.what());
        return -5;
    }
    catch(...)
    {
        LOGA("[GWJ] %s: Redis Unknown Exception", __FUNCTION__);
        return -5;
    }
}

int RedisModule::get_value_by_key(const char* key, string& value)
{
    if(this->conn == NULL)
    {
        LOGD("[GWJ] %s: get redis connection failed", __FUNCTION__);
        return -5;
    }

    try
    {
        this->reply = (redisReply *)redisCommand(this->conn, "GET %s", key);

        if(this->reply != NULL)
        {
            if(IS_REDIS_REPLY_ERROR(this->reply->type))
            {
                LOGD("[GWJ] %s: REDIS ERROR", __FUNCTION__);
                return -5;
            }
            else
            {
                if(IS_VALUED_STRING(this->reply->str))
                {
                    value = this->reply->str;
                    LOGD("[GWJ] %s: value[%s]", __FUNCTION__, value.c_str());
                    return 0;
                }
                else
                {
                    value = "";
                    LOGD("[GWJ] %s: Empty Reply", __FUNCTION__);
                    return -5;
                }
            }
        }
        else
        {
            LOGD("[GWJ] %s: get a NULL redis reply", __FUNCTION__);
            return -5;
        }
    }
    catch(exception& e)
    {
        LOGA("[GWJ] %s: Redis Exception[%s]", __FUNCTION__, e.what());
        return -5;
    }
    catch(...)
    {
        LOGA("[GWJ] %s: Redis Unknown Exception", __FUNCTION__);
        return -5;
    }
}

int RedisModule::get_value_list(const char* key, list<string>& values)
{
	int count = 0;

    if(this->conn == NULL)
    {
        LOGD("[GWJ] %s: get redis connection failed", __FUNCTION__);
        return -5;
    }

    redisReply *childReply = NULL;

    try
    {
        this->reply = (redisReply *)redisCommand(this->conn, "LLEN %s", key);

        if(this->reply != NULL)
        {
            if(IS_REDIS_REPLY_ERROR(this->reply->type))
            {
                LOGD("[GWJ] %s: REDIS ERROR", __FUNCTION__);
				freeReplyObject(childReply);
				childReply = NULL;
                return -5;
            }
            else
            {
				count  = this->reply->integer;
                LOGD("[GWJ] %s: set count is %d", __FUNCTION__, count);

                for(int i = 0; i < count; i++)
                {
					childReply = (redisReply *)redisCommand(this->conn, "LINDEX %s %d", key, i);

                    if(IS_VALUED_STRING(childReply->str))
                    {
                        string strCid(childReply->str);
                        LOGD("[GWJ] %s: push value[%s]", __FUNCTION__, strCid.c_str());
                        values.push_back(strCid);
                    }
                    else
                    {
                        LOGA("[GWJ] %s: ChildReply-str NULL!", __FUNCTION__);
                    }
                }
                return values.size();
            }
        }
        else
        {
            LOGD("[GWJ] %s: get a NULL redis reply", __FUNCTION__);
            return -5;
        }
    }
    catch(exception& e)
    {
        LOGA("[GWJ] %s: Redis Exception[%s]", __FUNCTION__, e.what());
        return -5;
    }
    catch(...)
    {
        LOGA("[GWJ] %s: Redis Unknown Exception", __FUNCTION__);
        return -5;
    }

}

int RedisModule::get_keys(const char* like_key, list<string>& keys)
{
    int count = 0;

    if(this->conn == NULL)
    {
        LOGD("[GWJ] %s: get redis connection failed", __FUNCTION__);
        return -5;
    }

    redisReply *childReply = NULL;

    try
    {
        this->reply = (redisReply *)redisCommand(this->conn, "KEYS %s", like_key);

        if(this->reply != NULL)
        {
            if(IS_REDIS_REPLY_ERROR(this->reply->type))
            {
                LOGD("[GWJ] %s: REDIS ERROR", __FUNCTION__);
                return -5;
            }
            else
            {
                count = this->reply->elements;
                LOGD("[GWJ] %s: set count is %d", __FUNCTION__, count);

                for(int i = 0; i < count; i++)
                {
                    childReply = this->reply->element[i];

                    if(IS_VALUED_STRING(childReply->str))
                    {
                        string strCid(childReply->str);
                        LOGD("[GWJ] %s: push value[%s]", __FUNCTION__, strCid.c_str());
                        keys.push_back(strCid);
                    }
                    else
                    {
                        LOGA("[GWJ] %s: ChildReply-str NULL!", __FUNCTION__);
                    }
                }

                return keys.size();
            }
        }
        else
        {
            LOGD("[GWJ] %s: get a NULL redis reply", __FUNCTION__);
            return -5;
        }
    }
    catch(exception& e)
    {
        LOGA("[GWJ] %s: Redis Exception[%s]", __FUNCTION__, e.what());
        return -5;
    }
    catch(...)
    {
        LOGA("[GWJ] %s: Redis Unknown Exception", __FUNCTION__);
        return -5;
    }
}

int RedisModule::get_values_by_key(const char* key, list<string>& values)
{
    int count = 0;

    if(this->conn == NULL)
    {
        LOGD("[GWJ] %s: get redis connection failed", __FUNCTION__);
        return -5;
    }

    redisReply *childReply = NULL;

    try
    {
        this->reply = (redisReply *)redisCommand(this->conn, "SMEMBERS %s", key);

        if(this->reply != NULL)
        {
            if(IS_REDIS_REPLY_ERROR(this->reply->type))
            {
                LOGD("[GWJ] %s: REDIS ERROR", __FUNCTION__);
                return -5;
            }
            else
            {
                count = this->reply->elements;
                LOGD("[GWJ] %s: set count is %d", __FUNCTION__, count);

                for(int i = 0; i < count; i++)
                {
                    childReply = this->reply->element[i];

                    if(IS_VALUED_STRING(childReply->str))
                    {
                        string strCid(childReply->str);
                        LOGD("[GWJ] %s: push value[%s]", __FUNCTION__, strCid.c_str());
                        values.push_back(strCid);
                    }
                    else
                    {
                        LOGA("[GWJ] %s: ChildReply-str NULL!", __FUNCTION__);
                    }
                }

                return values.size();
            }
        }
        else
        {
            LOGD("[GWJ] %s: get a NULL redis reply", __FUNCTION__);
            return -5;
        }
    }
    catch(exception& e)
    {
        LOGA("[GWJ] %s: Redis Exception[%s]", __FUNCTION__, e.what());
        return -5;
    }
    catch(...)
    {
        LOGA("[GWJ] %s: Redis Unknown Exception", __FUNCTION__);
        return -5;
    }
}

int RedisModule::get_values_count_by_key(const char* key)
{
    if(this->conn == NULL)
    {
        LOGD("[GWJ] %s: get redis connection failed", __FUNCTION__);
        return -5;
    }

    try
    {
        this->reply = (redisReply*)redisCommand(this->conn, "SMEMBERS %s", key);

        if(this->reply != NULL)
        {
            if(IS_REDIS_REPLY_ERROR(this->reply->type))
            {
                LOGD("[GWJ] %s: REDIS ERROR", __FUNCTION__);
                return -5;
            }
            else
            {
                int count = this->reply->elements;
                LOGD("[GWJ] %s: set count is %d", __FUNCTION__, count);
                return count;
            }
        }
        else
        {
            LOGD("[GWJ] %s: get a NULL redis reply", __FUNCTION__);
            return -5;
        }
    }
    catch(exception& e)
    {
        LOGA("[GWJ] %s: Redis Exception[%s]", __FUNCTION__, e.what());
        return -5;
    }
    catch(...)
    {
        LOGA("[GWJ] %s: Redis Unknown Exception", __FUNCTION__);
        return -5;
    }
}
