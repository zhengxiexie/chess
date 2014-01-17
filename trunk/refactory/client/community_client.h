#ifndef _COMMUNITY_H__
#define _COMMUNITY_H__

#include "chess_hall_props.idl.h"
#include "/home/work/public/ubrpc/client/client.h"
#include "Configure.h"

using namespace std;

class CommunityClient
{
    public:
    CommunityClient()
    {   
    }   
    ~CommunityClient()
    {   
        _mgr.close();
    }   
    int init();
    int obtain_commodity(int32_t userId, int32_t commodity_id, int32_t num, string gameId, bool& success);
    int obtain_exchange_code(int32_t user_id,string number,int32_t commodity_id, string expired_time, int32_t expired_type, string game_id, bool& success);
    int use_exchange_code(int32_t userId,string gameId, string exchangeCodeId, idl::result_cp& resultCP);

    private:
    //static MessageConnPool _conn_pool;
    ub::UbClientManager _mgr;
};

#endif
