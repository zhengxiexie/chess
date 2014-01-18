#include "community_client.h"
#include "Configure.h"
#include "chess_hall_props.idl.h"
#include "mcpsvr.h"
#include "../common.h"

//#include "client/client.h"

int CommunityClient::init()
{
    comcfg::Configure conf;
    if (0 != conf.load("./conf", "community_client.conf"))
    {
        UB_LOG_FATAL("open config error");
        return -1;
    }


    if (0 != _mgr.load(conf["UbClientConfig"]))
    {
        UB_LOG_FATAL("init UbClientManager error");
        return -1;
    }   
    return 0;
}

int CommunityClient::obtain_commodity(int32_t userId, int32_t commodity_id, int32_t num, string gameId, bool& success)
{
    UB_LOG_NOTICE( "CommunityClient::obtain_commodity start");
    if(this->init()!=0){
        UB_LOG_NOTICE( "CommunityClient init failed, [%s:%d]", __FILE__, __LINE__ );
        return -1;
    }

    ubrpc::NonblockClient client(&_mgr);
    ubrpc::Client client1(&_mgr);
    bsl::syspool pool;

    idl::chesshallprops_obtain_commodity_params pin(&pool);
    idl::chesshallprops_obtain_commodity_response pout(&pool);

    pin.set_userId(userId);
    pin.set_commodity_id(commodity_id);
    pin.set_num(num);
    pin.set_gameId(gameId.c_str());
    LOGA("obtain_commodity  commodity_id is %d", commodity_id);

    client.call(&client1, "Community", "obtain_commodity", &pin, &pout);
    client.waitAll();

    int res = client1.getError();
    if (0 != res ){
        UB_LOG_NOTICE("CommunityClient call obtain_commodity failed, userId[%ld], commodity_id[%d] num[%d] gameId[%s] [%s:%d]", userId, commodity_id, num, gameId.c_str(), __FILE__, __LINE__ );
        UB_LOG_NOTICE( "msg[%s]", client1.getErrorMessage());
        return -1;
    }else{
        success = pout.result_params().success();
        if(success){
            UB_LOG_NOTICE( "obtain_commodity result[true]" );
        }else{
            UB_LOG_NOTICE( "obtain_commodity result[false]" );
        }
    }

    return 0; 
}

int CommunityClient::obtain_exchange_code(int32_t user_id,string number,int32_t commodity_id, string expired_time, int32_t expired_type, string game_id, bool& success)
{
    UB_LOG_NOTICE( "CommunityClient::obtain_exchange_code start");
    if(this->init()!=0){
        UB_LOG_FATAL( "CommunityClient init failed, [%s:%d]", __FILE__, __LINE__ );
        return -1; 
    }   

    ubrpc::NonblockClient client(&_mgr);
    ubrpc::Client client1(&_mgr);
    bsl::syspool pool;

    idl::chesshallprops_obtain_exchange_code_params pin(&pool);
    idl::chesshallprops_obtain_exchange_code_response pout(&pool);

    pin.set_user_id(user_id);
    pin.set_number(number.c_str());
    pin.set_commodity_id(commodity_id);
    pin.set_expired_time(expired_time.c_str());
    pin.set_expired_type(expired_type);
    pin.set_game_id(game_id.c_str());
    /*
    */
    client.call(&client1,"Community", "obtain_exchange_code", &pin, &pout);
    client.waitAll();

    int res = client1.getError();
    if (0 != res ){
        UB_LOG_FATAL("CommunityClient call obtain_exchange_code failed, user_id[%d] number[%s] commodity_id[%d] expired_time[%s] game_id[%d] [%s:%d]", user_id, number.c_str(), commodity_id, expired_time.c_str(), game_id.c_str(), __FILE__, __LINE__ );
        UB_LOG_FATAL( "msg[%s]", client1.getErrorMessage());
        return -1;
    }else{
        bool success = pout.result_params().success();
        UB_LOG_FATAL("CommunityClient call obtain_exchange_code success, user_id[%d] number[%s] commodity_id[%d] expired_time[%s] game_id[%d] [%s:%d]", user_id, number.c_str(), commodity_id, expired_time.c_str(), game_id.c_str(), __FILE__, __LINE__ );
        UB_LOG_TRACE("use_exchange_code return success[%d] ", success?1:0);
    }
    UB_LOG_NOTICE( "CommunityClient::obtain_exchange_code end" );

    return 0; 
}

int CommunityClient::use_exchange_code(int32_t userId,string gameId, string exchangeCodeId, idl::result_cp& resultCP)
{
    UB_LOG_NOTICE( "CommunityClient::use_exchange_code start");
    if(this->init()!=0){
        UB_LOG_FATAL( "CommunityClient init failed, [%s:%d]", __FILE__, __LINE__ );
        return -1; 
    }   

    ubrpc::NonblockClient client(&_mgr);
    ubrpc::Client client1(&_mgr);
    bsl::syspool pool;

    idl::chesshallprops_use_exchange_code_params pin( &pool );
    idl::chesshallprops_use_exchange_code_response pout( &pool );

    pin.set_userId( userId );
    pin.set_gameId( gameId.c_str() );
    pin.set_exchangeCodeId( exchangeCodeId.c_str() );


    client.call(&client1,"Community", "use_exchange_code", &pin, &pout);
    client.waitAll();

    int res = client1.getError();
    if (0 != res ){
        UB_LOG_FATAL("CommunityClient call use_exchange_code failed, user_id[%ld] gameId[%s]  exchangeCodeId[%s] [%s:%d]", userId, gameId.c_str(), exchangeCodeId.c_str(), __FILE__, __LINE__ );
        UB_LOG_FATAL( "msg[%s]", client1.getErrorMessage());
        return -1;
    }else{
        const ::idl::result_cp rc = pout.mutable_result_params()->resultCP();
        UB_LOG_TRACE("use_exchange_code return is_success[%d] code[%d] message[%s]", rc.is_success(), rc.code(), rc.message());
    }
    UB_LOG_NOTICE( "CommunityClient::use_exchange_code end" );

    return 0; 
}

