#include "duoku_paradise.h"
#include "duokoo_mysql.h"

using namespace content;

string DuokuParadise::getRecentlyPlayedKey()
{
	return "recently_played_" + _u_id;
}

string DuokuParadise::getFavoriteKey()
{
	return "favorite_" + _u_id;
}

int DuokuParadise::setRecentlyPlayed()
{
	int nListLen = 0, i;
	string key=getRecentlyPlayedKey();
	redisContext *c; 
	c=redisConnPool::getInstance()->getConnection();
	if(c==NULL){
		UB_LOG_FATAL( "get redis connection failed, [%s:%d]", __FILE__, __LINE__ );
		return -5; 
	}

	redisReply *reply;
	reply=(redisReply *)redisCommand( c, "LLEN %s", key.c_str());
	if(reply!=NULL){
        if(reply->type==REDIS_REPLY_ERROR){
            UB_LOG_FATAL( "REDIS ERROR[%s]", reply->str );
			if(reply != NULL)
			{   
				freeReplyObject(reply);
			} 
			redisConnPool::getInstance()->releaseConnection(c);
			return -5; 
		}   
		else{
			nListLen = reply->integer;
			UB_LOG_TRACE("list len is %d", nListLen);
			freeReplyObject(reply);
		}
	}else{
		UB_LOG_FATAL( "get a NULL redis connection, [%s:%d]", __FILE__, __LINE__ );
		redisConnPool::getInstance()->releaseConnection(c);
		return -5; 
	}   	

	if(nListLen == 0)
	{
		nListLen = getRecentlyPlayedMysql2Redis();
	}

	UB_LOG_TRACE("LENGTH IS %d", nListLen);
	while(nListLen >4){
		UB_LOG_TRACE("LPOP here1");
		reply=(redisReply *)redisCommand( c, "LPOP %s", key.c_str());
		if(reply != NULL)
		{
			freeReplyObject(reply);
		}
		nListLen--;
	}

	for(i=0; i<nListLen; i++)
	{
		reply=(redisReply *)redisCommand(c,"LINDEX %s %d", key.c_str(), i);
		if(reply!=NULL && reply->type==REDIS_REPLY_STRING)
		{   
			if( strcmp(reply->str, _c_id.c_str()) == 0 )
			{
				freeReplyObject(reply);
				redisConnPool::getInstance()->releaseConnection(c);
				return 0;
			}
		}
	}
	
	if(i==4)
	{
		reply=(redisReply *)redisCommand( c, "LPOP %s", key.c_str());
		UB_LOG_TRACE("LPOP here2");
	}
	reply=(redisReply *)redisCommand( c, "RPUSH %s %s", key.c_str(), _c_id.c_str());
	if(reply != NULL)
	{
		freeReplyObject(reply);
	}

	/*
	reply=(redisReply *)redisCommand(c,"LINDEX %s %d", key.c_str(), -1);

	if(reply!=NULL && reply->type==REDIS_REPLY_STRING)
	{
		if( strcmp(reply->str, _c_id.c_str()) )
		{
			if(nListLen == 4)
			{
				reply=(redisReply *)redisCommand( c, "LPOP %s", key.c_str());
				if(reply != NULL)
				{
					freeReplyObject(reply);
				}
			}
			reply=(redisReply *)redisCommand( c, "RPUSH %s %s", key.c_str(), _c_id.c_str());
			if(reply != NULL)
			{   
				freeReplyObject(reply);
			} 
		}
	}
	else
	{
		reply=(redisReply *)redisCommand( c, "RPUSH %s %s", key.c_str(), _c_id.c_str());
		if(reply != NULL)
		{   
			freeReplyObject(reply);
		} 
	}
	*/
	redisConnPool::getInstance()->releaseConnection(c);

	return 0;
}

int DuokuParadise::getRecentlyPlayed(vector<string> &vsCids)
{
	int nListLen = 0;
	string key=getRecentlyPlayedKey();

	UB_LOG_TRACE("key is %s", key.c_str());
	redisContext *c;
	c=redisConnPool::getInstance()->getConnection();
	if(c==NULL){
		UB_LOG_FATAL( "get redis connection failed, [%s:%d]", __FILE__, __LINE__ );
		return -5; 
	} 

	redisReply *reply;
	reply=(redisReply *)redisCommand( c, "LLEN %s", key.c_str());

	if(reply!=NULL){
		if(reply->type==REDIS_REPLY_ERROR){
			UB_LOG_FATAL( "REDIS ERROR[%s]", reply->str );
			freeReplyObject(reply);
			redisConnPool::getInstance()->releaseConnection(c);
			return -5; 
		}   
		else{
			nListLen = reply->integer;
		}   
	}else{
		UB_LOG_FATAL( "get a NULL redis connection, [%s:%d]", __FILE__, __LINE__ );
		redisConnPool::getInstance()->releaseConnection(c);
		return -5; 
	}

	UB_LOG_TRACE("list len is %d", nListLen);
	if(nListLen == 0)
	{
		//nListLen = getRecentlyPlayedMongo2Redis();
		nListLen = getRecentlyPlayedMysql2Redis();
	}

	UB_LOG_TRACE("list len is %d", nListLen);
	int i=0;
	while(i<nListLen)
	{
		reply=(redisReply *)redisCommand( c, "LINDEX %s %d", key.c_str(), i);
		string strCid(reply->str);
		vsCids.push_back(strCid);
		i++;
	}
	if(reply!=NULL)
	{
		freeReplyObject(reply);
	}
	redisConnPool::getInstance()->releaseConnection(c);
	return 0;
}

int DuokuParadise::setFavorite()
{
	string key=getFavoriteKey();

	UB_LOG_TRACE("key is %s", key.c_str());
	redisContext *c;
	c=redisConnPool::getInstance()->getConnection();

	if(c==NULL){
		UB_LOG_FATAL( "get redis connection failed, [%s:%d]", __FILE__, __LINE__ );
		return -5; 
	}

	redisReply *reply;
	uint32_t ncid = atoi(_c_id.c_str());
	reply=(redisReply *)redisCommand( c, "SADD %s %d", key.c_str(), ncid);
	if(reply!=NULL){
		if(reply->type==REDIS_REPLY_ERROR){
			UB_LOG_FATAL( "REDIS ERROR[%s]", reply->str );
			redisConnPool::getInstance()->releaseConnection(c);
			return -5; 
		}   
		freeReplyObject(reply);
	}else{
		UB_LOG_FATAL( "get a NULL redis connection, [%s:%d]", __FILE__, __LINE__ );
		redisConnPool::getInstance()->releaseConnection(c);
		return -5; 
	}
	redisConnPool::getInstance()->releaseConnection(c);
	return 0;	
}

int DuokuParadise::getFavorite(vector<string> &vs)
{
	UB_LOG_TRACE("get favorite start");

	int count=0;
	string key=getFavoriteKey();
	UB_LOG_TRACE("key is %s", key.c_str());

	redisContext *c; 
	c=redisConnPool::getInstance()->getConnection();

	if(c==NULL){
		UB_LOG_FATAL( "get redis connection failed, [%s:%d]", __FILE__, __LINE__ );
		return -5; 
	} 

	redisReply *reply, *childReply;
	UB_LOG_TRACE("key is %s", key.c_str());
	reply=(redisReply *)redisCommand( c, "SMEMBERS %s", key.c_str());

	if(reply!=NULL){
		if(reply->type==REDIS_REPLY_ERROR){
			UB_LOG_TRACE("redis reply error");
			UB_LOG_FATAL( "REDIS ERROR[%s]", reply->str );
			//freeReplyObject(reply);
			redisConnPool::getInstance()->releaseConnection(c);
			return -5; 
		}   
	}else{
		UB_LOG_TRACE("redis connection null");
		UB_LOG_FATAL( "get a NULL redis connection, [%s:%d]", __FILE__, __LINE__ );
		redisConnPool::getInstance()->releaseConnection(c);
		return -5; 
	}

	UB_LOG_TRACE("set count is %d", count);
	count = reply->elements;


	if(count == 0)
	{
		count = this->getFavoriteMysql2Redis();	
	}

	UB_LOG_TRACE("set count is %d", count);

	reply=(redisReply *)redisCommand( c, "SMEMBERS %s", key.c_str());
	count = reply->elements;

	for(int i=0; i< count; i++)
	{
		childReply = reply->element[i];
		string strCid(childReply->str);
		vs.push_back(strCid);	
	}
	if(childReply!=NULL)
	{
		;//freeReplyObject(childReply);
	}
	UB_LOG_TRACE("get favorite end");
	if(reply != NULL)
	{
		freeReplyObject(reply);
	}

	redisConnPool::getInstance()->releaseConnection(c);
	/*
	   int count = 2;
	   string s("47621");
	   vs.push_back(s);
	   string s1("47620");
	   vs.push_back(s1);
	   */
	return count;
}

int DuokuParadise::getRecentlyPlayedMysql2Redis()
{
	DuokooMysql mysql;
	string query_sql("select `c_id` from `MCP`.`mcp_content_recently_played` where `enable` = 1 and `u_id` = ");
	query_sql.append(_u_id);
	query_sql.append(" order by `priority`");

	int count=mysql.query(query_sql);

	if(count<0){
		UB_LOG_FATAL( "sql[%s], [%s:%d]", query_sql.c_str(), __FILE__, __LINE__ );
		return -5; 
	}else if(count==0){
		UB_LOG_FATAL( "user id[%s] has no record, [%s:%d]", _u_id.c_str(), __FILE__, __LINE__ );
		return -5; 
	}  

	vector<uint32_t> vi;
	for(int i=0; i<count; i++)
	{
		uint32_t cid = (uint32_t)atoi(mysql.getResult(i,0).c_str());
		vi.push_back(cid);
	}

	if(vi.size() > 0)
	{
		redisContext *c;
		c=redisConnPool::getInstance()->getConnection();
		redisReply *reply;
		string key = getRecentlyPlayedKey();

		if(c==NULL)
		{
			UB_LOG_FATAL( "redis conn is null");
			return -5;
		}
		for(int i=0; i<(int)vi.size(); i++)
		{
			UB_LOG_TRACE("key %d is %d", i, vi[i]);
			reply=(redisReply *)redisCommand( c, "RPUSH %s %d", key.c_str(), vi[i]);
		}
		if(reply != NULL)
		{
			freeReplyObject(reply);
		}
		redisConnPool::getInstance()->releaseConnection(c);
	}

	return (int)vi.size();
}

int DuokuParadise::getFavoriteMysql2Redis()
{
	DuokooMysql mysql;
	UB_LOG_TRACE("get favorite mysql 2 redis start");
	string query_sql("select `c_id` from `MCP`.`mcp_content_favorite_list` where `enable` = 1 and `u_id` = ");
	query_sql.append(_u_id);
	query_sql.append(" order by `priority`");

	int count=mysql.query(query_sql);
	UB_LOG_TRACE("mysql query count is %d", count);

	if(count<0){
		UB_LOG_FATAL( "sql[%s], [%s:%d]", query_sql.c_str(), __FILE__, __LINE__ );
		return -5; 
	}else if(count==0){
		UB_LOG_FATAL( "user id[%s] has no record, [%s:%d]", _u_id.c_str(), __FILE__, __LINE__ );
		return -5; 
	}  

	vector<uint32_t> vi;
	for(int i=0; i<count; i++)
	{
		uint32_t cid = (uint32_t)atoi(mysql.getResult(i,0).c_str());
		UB_LOG_TRACE("cid is %d", (int)cid);
		vi.push_back(cid);
	}

	if(vi.size() > 0)
	{
		redisContext *c;
		c=redisConnPool::getInstance()->getConnection();
		redisReply *reply;
		string key = getFavoriteKey();
		if(c==NULL)
		{
			UB_LOG_FATAL( "redis conn is null");
			return -5;
		}

		for(int i=0; i<(int)vi.size(); i++)
		{
			reply=(redisReply *)redisCommand( c, "SADD %s %d", key.c_str(), vi[i]);
		}
		if(reply != NULL)
		{
			freeReplyObject(reply);
		}
		redisConnPool::getInstance()->releaseConnection(c);
	}

	UB_LOG_TRACE("get favorite mysql 2 redis end");
	return (int)vi.size();
}

