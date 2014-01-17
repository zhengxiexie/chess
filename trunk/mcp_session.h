#ifndef _MCP_SESSION_H_
#define _MCP_SESSION_H_

#include "hiredis.h"
#include <iostream>
#include <algorithm>
#include <stdio.h>
#include <string>
#include <map>
#include <vector>
#include <time.h>
#include <sys/types.h>
#include <regex.h>
#include "mcp.idl.mcp.h"
#include "json/json.h"

using namespace std;

extern uint32_t g_session_tmout;
extern uint32_t g_redis_db;
extern char REDIS_HOST[16];
extern char REDIS_PORT[6];


class mcp_session 
{
public:
    mcp_session():_reply( NULL )
    {
        struct timeval timeout = { 1, 500000 };
        _c = redisConnectWithTimeout((char*)REDIS_HOST, atoi(REDIS_PORT), timeout);
        _ss_time_out = g_session_tmout;
        _ck_time_out = 0;
        _reply = (redisReply *)redisCommand( _c, "select %d", g_redis_db );
        if( _reply != NULL )
        {
            freeReplyObject(_reply);
            _reply = NULL;
        }
    }
    mcp_session( const uint32_t cookie_tmout ):_reply( NULL )
    {
        struct timeval timeout = { 1, 500000 };
        _c = redisConnectWithTimeout((char*)REDIS_HOST, atoi(REDIS_PORT), timeout);
        _ck_time_out = cookie_tmout;
        _ss_time_out = g_session_tmout;
        _reply = (redisReply *)redisCommand( _c, "select %d", g_redis_db );
        if( _reply != NULL )
        {
            freeReplyObject(_reply);
            _reply = NULL;
        }
    }
    ~mcp_session()
    {
        if( _c != NULL )
        {
            redisFree( _c );
        }
        if( _reply != NULL )
        {
            freeReplyObject(_reply);
        }
    }
    int err()
    {
        return _c->err;
    }
    char* get_err_str()
    {
        return _c->errstr;
    }
    int session_delete(const string& mainkey, const string& subkey);
    int session_insert(const string& mainkey, const string& subkey, const string& value, const int timeout);
    int session_search(const string& mainkey, const string& subkey, string& valueout);

private:
    redisContext *_c;
    redisReply *_reply;
    uint32_t _ss_time_out;   //session time out(second)
    uint32_t _ck_time_out;   //cookie time out( second )
};

#endif
