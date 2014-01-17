#include "grab.h"
#include "duokoo_mysql.h"
#include "redis_connpool.h"
#include "./refactory/common.h"

int Grab::getGrabInfo(grab_t* grab_info)
{
    UB_LOG_TRACE("Grab::getGrabInfo start");
    string query_sql("SELECT `id`, `name`, `c_id`, `info`, `start_time`, `end_time`, `amount`, `probability` "
                     "FROM `MCP`.`mcp_content_grab` "
                     "WHERE `enable` = 1 AND `id` = " );
    query_sql.append(toString(_id));
    //DuokooMysql mysql;
    DuokooMysql mysql("grab_mysql");
    int count=mysql.query(query_sql);
    if(count<0){
        UB_LOG_FATAL( "sql[%s], [%s:%d]", query_sql.c_str(), __FILE__, __LINE__ );
        return -5;
    }else if(count==0){
        UB_LOG_FATAL( "Grab id[%d] has no record, [%s:%d]", _id, __FILE__, __LINE__ );
        return -5;
    }
    _info.clear();
    _info.id=atoi(mysql.getResult(0,0).c_str());
    _info.name=mysql.getResult(0,1);
    _info.c_id=mysql.getResult(0,2);
    _info.info=mysql.getResult(0,3);
    _info.start_time=mysql.getResult(0,4);
    _info.end_time=mysql.getResult(0,5);
    _info.amount=atoi(mysql.getResult(0,6).c_str());
    _info.probability=atof(mysql.getResult(0,7).c_str());
    _info.send_amount=this->getSendAmount();
    UB_LOG_NOTICE("name:[%s]",_info.name.c_str());
    if(grab_info!=NULL){
        *grab_info = _info;
    }

    UB_LOG_TRACE("Grab::getGrabInfo end");
    return 0;
}

int Grab::getSendAmount()
{
    redisContext *c;
    uint32_t send_amount(0);
    c=redisConnPool::getInstance()->getConnection();
    if(c==NULL){
        UB_LOG_FATAL( "get redis connection failed, [%s:%d]", __FILE__, __LINE__ );
        return -5;
    }
    redisReply *reply;
    UB_LOG_DEBUG( "HGET GRAB:%d send_amount ", _id );
    reply = (redisReply*)redisCommand( c, "HGET GRAB:%d send_amount ", _id );
    redisConnPool::getInstance()->releaseConnection(c);
    if(reply!=NULL){
        if(reply->type!=REDIS_REPLY_ERROR){
            if(reply->type!=REDIS_REPLY_NIL){
                UB_LOG_NOTICE( "send amount:[%s]", reply->str );
                send_amount=atoi(reply->str);
            }else{
                send_amount=0;
            }
        }else{
            UB_LOG_FATAL( "REDIS ERROR[%s]", reply->str );
			if(reply != NULL)
			{
				freeReplyObject(reply);
			}
            return -5;
        }
        freeReplyObject(reply);
    }else{
        UB_LOG_FATAL( "get a NULL redis connection, [%s:%d]", __FILE__, __LINE__ );
        return -5;
    }
    return send_amount;
}

int Grab::setSendAmount(const uint32_t amount)
{
    redisContext *c;
    c=redisConnPool::getInstance()->getConnection();
    if(c==NULL){
        UB_LOG_FATAL( "get redis connection failed, [%s:%d]", __FILE__, __LINE__ );
        return -5;
    }
    redisReply *reply;
    UB_LOG_DEBUG( "HSET GRAB:%d send_amount %d", _id, amount );
    reply = (redisReply*)redisCommand( c, "HSET GRAB:%d send_amount %d", _id, amount );
    redisConnPool::getInstance()->releaseConnection(c);
    if(reply!=NULL){
        if(reply->type==REDIS_REPLY_ERROR){
			UB_LOG_FATAL( "REDIS ERROR[%s]", reply->str );
			if(reply != NULL)
			{   
				freeReplyObject(reply);
			}
			return -5;
		}
		freeReplyObject(reply);
	}else{
		UB_LOG_FATAL( "get a NULL redis connection, [%s:%d]", __FILE__, __LINE__ );
		return -5;
	}
	return 0;
}

int Grab::incrSendAmount()
{
	redisContext *c;
	c=redisConnPool::getInstance()->getConnection();
	if(c==NULL){
		UB_LOG_FATAL( "get redis connection failed, [%s:%d]", __FILE__, __LINE__ );
		return -5;
	}
	redisReply *reply;
	UB_LOG_DEBUG( "HINCRBY GRAB:%d send_amount 1", _id );
	reply = (redisReply*)redisCommand( c, "HINCRBY GRAB:%d send_amount 1", _id );
	redisConnPool::getInstance()->releaseConnection(c);
	if(reply!=NULL){
		if(reply->type!=REDIS_REPLY_ERROR){
			_info.send_amount=reply->integer;
		}else{
			UB_LOG_FATAL( "REDIS ERROR[%s]", reply->str );
			if(reply != NULL)
			{   
				freeReplyObject(reply);
			}
			return -5;
		}
		freeReplyObject(reply);
	}else{
		UB_LOG_FATAL( "get a NULL redis connection, [%s:%d]", __FILE__, __LINE__ );
		return -5;
	}
	return 0;
}

int Grab::userGrab(const string& user_id, string& num)
{
	int res=this->getGrabInfo();
	if(res!=0){
		UB_LOG_FATAL( "getGrabInfo failed, [%s:%d]", __FILE__, __LINE__ );
		return -5;
	}
	time_t t;
	t=time(0);
	if(t<stringToTimestamp(_info.start_time)){
		UB_LOG_NOTICE( "_id[%d] does not start[%s]", _id, _info.start_time.c_str() );
		return -1;
	}
	if(t>stringToTimestamp(_info.end_time)){
		UB_LOG_NOTICE( "_id[%d] has already end[%s]", _id, _info.end_time.c_str() );
		return -2;
	}
	if(_info.send_amount>=_info.amount){
		UB_LOG_NOTICE( "_id[%d] has already send end. amount[%d],send_amount[%d]", _id, _info.amount, _info.send_amount );
		return -3;
	}
	res=this->getUserGrab(user_id, num);
	if(res<0){
		UB_LOG_FATAL("getUserGrab failed, [%s:%d]", __FILE__, __LINE__ );
		return -5;
	}else if(res>0){
		UB_LOG_NOTICE( "user[%s] has already get a num[%s] in grab[%d]", user_id.c_str(), num.c_str(), _id );
		return -6;
	}
	long random_num=getRandomNum(100);
	if(random_num<_info.probability){
		res=this->getGrabNum(user_id, num, 1);
		if(res==-5){
			UB_LOG_FATAL("getGrabNum failed, user_id[%s], [%s:%d]", user_id.c_str(), __FILE__, __LINE__ );
			num.clear();
			return -5;
		}else if(res==-3){
			UB_LOG_FATAL("getGrabNum has already send over, id[%d], user_id[%s], [%s:%d]", 
					_id, user_id.c_str(), __FILE__, __LINE__ );
			/*res = this->setSendAmount(_info.amount);*/
			//if(res!=0){
				//UB_LOG_FATAL( "setSendAmount failed, id[%d], [%s:%d]", _id, __FILE__, __LINE__ );
			//}
			/*num.clear();*/
			return -5;
		}else{
			res= this->incrSendAmount();
			if(res!=0){
				UB_LOG_FATAL( "incrSendAmount failed, id[%d], [%s:%d]", _id, __FILE__, __LINE__ );
			}
		}
	}else{
		return -4;
	}
	return 0;
}

int Grab::userGrabPre(const string& user_id, string& num)
{
	int res=this->getGrabInfo();
	if(res!=0){
		UB_LOG_FATAL( "getGrabInfo failed, [%s:%d]", __FILE__, __LINE__ );
		return -5;
	}
	time_t t;
	t=time(0);
    
	if(t>stringToTimestamp(_info.start_time)){
		UB_LOG_NOTICE( "_id[%d] has started, can not pre order [%s]", _id, _info.start_time.c_str() );
		return -1;
	}
    
	if(t>stringToTimestamp(_info.end_time)){
		UB_LOG_NOTICE( "_id[%d] has already end[%s]", _id, _info.end_time.c_str() );
		return -2;
	}
	if(_info.send_amount>=_info.amount){
		UB_LOG_NOTICE( "_id[%d] has already send end. amount[%d],send_amount[%d]", _id, _info.amount, _info.send_amount );
		return -3;
	}
	res=this->getUserGrab(user_id, num);
	if(res<0){
		UB_LOG_FATAL("getUserGrab failed, [%s:%d]", __FILE__, __LINE__ );
		return -5;
	}else if(res>0){
		UB_LOG_NOTICE( "user[%s] has already get a num[%s] in grab[%d]", user_id.c_str(), num.c_str(), _id );
		return -6;
	}
	//long random_num=getRandomNum(100);
    //if(random_num<_info.probability){
    res=this->getGrabNum(user_id, num, 2);
    if(res==-5){
        UB_LOG_FATAL("getGrabNum failed, user_id[%s], [%s:%d]", user_id.c_str(), __FILE__, __LINE__ );
        num.clear();
        return -5;
    }else if(res==-3){
        UB_LOG_FATAL("getGrabNum has already send over, id[%d], user_id[%s], [%s:%d]", 
                _id, user_id.c_str(), __FILE__, __LINE__ );
/*        res = this->setSendAmount(_info.amount);*/
        //if(res!=0){
            //UB_LOG_FATAL( "setSendAmount failed, id[%d], [%s:%d]", _id, __FILE__, __LINE__ );
        //}
        /*num.clear();*/
        return -5;
    }else{
        res= this->incrSendAmount();
        if(res!=0){
            UB_LOG_FATAL( "incrSendAmount failed, id[%d], [%s:%d]", _id, __FILE__, __LINE__ );
        }
    }
	//}else{
	//	return -4;
	//}
	return 0;
}


int Grab::getGrabNum(const string& user_id, string& num, int32_t is_pre)
{
	extern string IP;
	int res;
	string query_sql;
	//DuokooMysql mysql;
    DuokooMysql mysql("grab_mysql");
	if(!mysql.startTransaction()){
		return -5;
	}
	//get num from mysql
	//comment by zhengxie 2013.10.22
	/*query_sql="SELECT num FROM `MCP`.`mcp_content_grab_num` WHERE `is_occupy` = 0 AND `grab_id` = " + toString(_id) + " limit 0, 1";*/
	//UB_LOG_DEBUG("query_sql:[%s]", query_sql.c_str());
	//res=mysql.query(query_sql);
	//if(res<0){
		//UB_LOG_FATAL( "query sql[%s] failed, [%s], [%s:%d]", query_sql.c_str(), mysql.getErrDes().c_str(), __FILE__, __LINE__ );
		//mysql.rollback();
		//return -5;
	//}else if(res==0){
		//UB_LOG_FATAL( "query sql[%s] have no record, [%s:%d]", query_sql.c_str(), __FILE__, __LINE__ );
		//mysql.rollback();
		//return -3;
	//}
	/*num=mysql.getResult(0,0);*/

	/* 2013.10.22 add by zhengxie get num from mysql => get num from redis start */
	redisContext *c;
    uint32_t send_amount(0);
    c=redisConnPool::getInstance()->getConnection();
    if(c==NULL)
	{
        UB_LOG_FATAL( "get redis connection failed, [%s:%d]", __FILE__, __LINE__ );
        return -5;
    }
    redisReply *reply;
	/* 2013.12.03 add by zhengxie add lock */

	string thread_id = DuokooTools::toString(CommonInterface::get_thread_id());
	string random = DuokooTools::toString(getRandomNum(10000000));

	string key = "grab_and_user_id:" + DuokooTools::toString(_id) + "," + user_id;
	string value = IP + "," + thread_id + "," + random;

	redisCommand(c, "SETNX %s %s", key.c_str(), value.c_str());

	LOGD( "[zx]get_value key:%s ", key.c_str() );
    reply = (redisReply*)redisCommand( c, "GET %s", key.c_str() );

    redisConnPool::getInstance()->releaseConnection(c);
    if(reply!=NULL)
	{
        if(reply->type!=REDIS_REPLY_ERROR)
		{
            if(reply->type!=REDIS_REPLY_NIL)
			{
                string temp_value = reply->str;
				freeReplyObject(reply);
				reply = NULL;
				if(strcmp(temp_value.c_str(), value.c_str()))
				{
					LOGA( "[zx] shit, bad user[%s] ", user_id.c_str() );
					return -3;//同一个用户重复抢号
				}
            }
			else
			{
				freeReplyObject(reply);
				reply = NULL;
				return -3;
            }
		}
	}
	else
	{
		LOGA( "[zx] key not set[%s] ", key.c_str() );
		return -5;
	}

	/* 2013.12.03 add by zhengxie add lock */

	c=redisConnPool::getInstance()->getConnection();
    if(c==NULL)
	{
        UB_LOG_FATAL( "get redis connection failed, [%s:%d]", __FILE__, __LINE__ );
        return -5;
    }

	redisCommand(c, "EXPIRE %s %ld", key.c_str(), 10*24*60*60);
    //LOGD( "[zx]SPOP grab_id:%d ", _id );
    reply = (redisReply*)redisCommand( c, "SPOP grab_id:%d", _id );
    redisConnPool::getInstance()->releaseConnection(c);
    if(reply!=NULL)
	{
        if(reply->type!=REDIS_REPLY_ERROR)
		{
            if(reply->type!=REDIS_REPLY_NIL)
			{
                LOGA( "grab_id:[%d], user_id:[%s], grab_num:[%s]", _id, user_id.c_str(), reply->str );
                num=reply->str;
				freeReplyObject(reply);
				reply = NULL;
            }
			else
			{
				LOGD( "[zx] has no grab_nums grab_id[%d] ", _id );
				freeReplyObject(reply);
				reply = NULL;
				return -3;
            }
		}
	}
	else
	{
		LOGD( "[zx] not exist grab_id[%d] ", _id );
		return -5;
	}

	/* 2013.10.22 add by zhengxie get num from mysql => get num from redis end */
	//update num
	query_sql="UPDATE `MCP`.`mcp_content_grab_num` SET `is_occupy` = "+ toString(is_pre) +" WHERE num = '" + num + "' ";
	UB_LOG_DEBUG("query_sql:[%s]", query_sql.c_str());
	res=mysql.query(query_sql);
	if(res<0){
		UB_LOG_FATAL( "query sql[%s] failed, [%s], [%s:%d]", query_sql.c_str(), mysql.getErrDes().c_str(), __FILE__, __LINE__ );
		mysql.rollback();
		return -5;
	}
	//insert user_num
	query_sql="INSERT INTO `MCP`.`mcp_content_user_grab`( `id`, `grab_id`, `user_id`, `grab_time`, `use_time`, `num` )"
		"VALUES( NULL, '"+toString(_id)+"', '"+user_id+"', '"+NOW()+"', '0', '"+num+"' ) ";
	UB_LOG_DEBUG("query_sql:[%s]", query_sql.c_str());
	res=mysql.query(query_sql);
	if(res<0){
		UB_LOG_FATAL( "query sql[%s] failed, [%s], [%s:%d]", query_sql.c_str(), mysql.getErrDes().c_str(), __FILE__, __LINE__ );
		mysql.rollback();
		return -5;
	}
	return mysql.commit()?0:-5;
}

int Grab::getUserGrab(const string& user_id, string& num)
{
	int res(0);
	string query_sql;
	//DuokooMysql mysql;
    DuokooMysql mysql("grab_mysql");
	query_sql="SELECT num FROM `MCP`.`mcp_content_user_grab` WHERE `user_id` = " + user_id + " AND `grab_id` = " + toString(_id);
	res=mysql.query(query_sql);
	if(res<0){
		UB_LOG_FATAL( "query sql[%s] failed, [%s], [%s:%d]", query_sql.c_str(), mysql.getErrDes().c_str(), __FILE__, __LINE__ );
	}else if(res>0){
		UB_LOG_NOTICE( "user[%s] has already get a num[%s] in grab[%d]", user_id.c_str(), num.c_str(), _id );
		num = mysql.getResult(0,0);
	}
	return res;
}

int Grab::getUserGrabEx(const string& user_id, string& num, int32_t& occupy)
{
    int res(0);
    string query_sql;

    DuokooMysql mysql("grab_mysql");
    query_sql="SELECT num FROM `MCP`.`mcp_content_user_grab` WHERE `user_id` = " + user_id + " AND `grab_id` = " + toString(_id);
    res=mysql.query(query_sql);
    if(res<0){
        UB_LOG_NOTICE( "query sql[%s] failed, [%s], [%s:%d]", query_sql.c_str(), mysql.getErrDes().c_str(), __FILE__, __LINE__ );
        return res;
    }else if(res>0){
        UB_LOG_NOTICE( "user[%s] has already get a num[%s] in grab[%d]", user_id.c_str(), num.c_str(), _id );
        num = mysql.getResult(0,0);
    }   

    query_sql = "SELECT is_occupy from mcp_content_grab_num where num = '" + num + "'";
    res = mysql.query(query_sql);
    if(res<0){
        UB_LOG_NOTICE("query sql[%s] failed, [%s], [%s:%d]", query_sql.c_str(), mysql.getErrDes().c_str(), __FILE__, __LINE__ );
        return res;
    }else if(res>0){
        res = 0;
        occupy = atoi(mysql.getResult(0,0).c_str());
        UB_LOG_NOTICE("occupy is %d");
    }

    return res;
}
