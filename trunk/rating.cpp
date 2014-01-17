#include "rating.h"
#include "redis_connpool.h"
#include "appinfo_content.h"

using namespace content;

int Rating::setRating(const string& user_id, const uint32_t rating)
{
	redisContext *c;
	c=redisConnPool::getInstance()->getConnection();
	if(c==NULL){
		UB_LOG_FATAL( "get redis connection failed, [%s:%d]", __FILE__, __LINE__ );
		return -5;
	}
	redisReply *reply;
	UB_LOG_DEBUG( "HSET %s rating %d ", getRedisKey(user_id).c_str(), rating );
	reply=(redisReply *)redisCommand( c, "HSET %s rating %d ", getRedisKey(user_id).c_str(), rating );
	redisConnPool::getInstance()->releaseConnection(c);
	if(reply!=NULL){
		if(reply->type==REDIS_REPLY_ERROR){
			UB_LOG_FATAL( "REDIS ERROR[%s]", reply->str );
			return -5;
		}
		freeReplyObject(reply);
	}else{
		UB_LOG_FATAL( "get a NULL redis connection, [%s:%d]", __FILE__, __LINE__ );
		return -5;
	}
	return 0;
}

int Rating::getRating(const string& user_id)
{
	redisContext *c;
	c=redisConnPool::getInstance()->getConnection();
	if(c==NULL){
		UB_LOG_FATAL( "get redis connection failed, [%s:%d]", __FILE__, __LINE__ );
		return -5;
	}
	int rating;
	redisReply *reply;
	UB_LOG_DEBUG( "HGET %s rating ", getRedisKey(user_id).c_str() );
	reply=(redisReply *)redisCommand( c, "HGET %s rating ", getRedisKey(user_id).c_str() );
	redisConnPool::getInstance()->releaseConnection(c);
	if(reply!=NULL){
		if(reply->type!=REDIS_REPLY_ERROR){
			if(reply->type==REDIS_REPLY_STRING){
				rating=atoi(reply->str);
			}else{
				rating=0;
			}
		}else{
			UB_LOG_FATAL( "REDIS ERROR[%s]", reply->str );
			return -5;
		}
		freeReplyObject(reply);
	}else{
		UB_LOG_FATAL( "get a NULL redis connection, [%s:%d]", __FILE__, __LINE__ );
		return -5;
	}
	return rating;
}

string Rating::getRedisKey(const string& user_id)
{
	if(_package_name.empty()){
		return "Cid:" + _c_id + ":UserId:" + user_id;
	}
	return _package_name + ":UserId:" + user_id;
}

string Rating::getPackageNameByCid(const string& c_id)
{
	AppinfoContent app_info(atoi(c_id.c_str()));
	appinfo_t info;
	if(app_info.get_info(info)!=0){
		UB_LOG_FATAL( "AppinfoContent::get_info failed, [%s:%d]", __FILE__, __LINE__ );
		return string();
	}
	return info.package_name;
}

