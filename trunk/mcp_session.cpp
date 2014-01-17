#include "mcp_session.h"

int mcp_session::session_delete(const string& mainkey, const string& subkey)
{
  string tmp = mainkey + subkey;
  _reply = (redisReply *)redisCommand( _c, "DEL %s", tmp.c_str() );
  if( _reply != NULL )
  {
     freeReplyObject(_reply);
     _reply = NULL;
  } 
   return 0;
}

int mcp_session::session_insert(const string& mainkey, const string& subkey, const string& value, const int timeout)
{
   string tmp = mainkey + subkey;
   _reply = (redisReply *)redisCommand( _c, "SET %s %s", tmp.c_str(), value.c_str() );
   if( _reply != NULL )
   {
      freeReplyObject(_reply);
      _reply = NULL;
   }
   _reply = (redisReply *)redisCommand( _c, "EXPIRE %s %d", tmp.c_str(), timeout );
   if( _reply != NULL )
   {
      freeReplyObject(_reply);
      _reply = NULL;
   }
   return 0;
}

int mcp_session::session_search(const string& mainkey, const string& subkey, string& valueout)
{
    string tmp = mainkey + subkey;
    _reply = (redisReply *)redisCommand( _c, "GET %s", tmp.c_str() );
    if( _reply != NULL && _reply->type == 1 )
    {
       valueout = _reply->str;
    }
    else
    {
       //UB_LOG_WARNING( "GET %s %s failed, [%s:%d]", mainkey.c_str(), subkey.c_str(), __FILE__, __LINE__ );
       if( _reply != NULL )
       {
	  freeReplyObject(_reply);
	  _reply = NULL;
       }
       valueout = "";
       return -1;
    }
   return 0;
}
