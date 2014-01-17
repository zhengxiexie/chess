/*
 * middle_client.cpp
 *
 *  Created on: 2013-09-04
 *      Author: zhengxie
 */

#include "middle_client.h"

MiddleClient::MiddleClient()
{
    this->init();
}

int MiddleClient::init()
{
    comcfg::Configure conf;

    if (0 != conf.load("./conf", "middle_client.conf"))
    {
        LOGN("[GWJ] %s: Notify_RPC Init Error!", __FUNCTION__);
        return -1;
    }

    if (0 != this->_mgr.load(conf["UbClientConfig"]))
    {
        LOGN("[GWJ] %s: Load Notify_RPC Conf Error!", __FUNCTION__);
        return -1;
    }

    return 0;
}


int MiddleClient::addGameHallScore(uint32_t userId, uint32_t opId)
{
    LOGN("[GWJ] %s: start", __FUNCTION__);

    ubrpc::NonblockClient client(&_mgr);
    ubrpc::Client client1(&_mgr);
    bsl::syspool pool;

    int res(0);
    idl::GameHallPoint_addGameHallScore_params pin( &pool );
    idl::GameHallPoint_addGameHallScore_response pout( &pool );

    pin.set_userId( userId );
    pin.set_opId( opId );

    client.call( &client1, "GameHallPoint", "addGameHallScore", &pin, &pout );
    client.waitAll();

    res = client1.getError();

    if(0 != res)
    {
        LOGN("[GWJ] %s: start. Notice Failed!!! [uer_id:%d],[opt:%d]",
                __FUNCTION__, userId, optId);

        return -1;
    }

    int32_t score = pout.result_params().score();

    LOGN("[GWJ] %s: end[score:%d]", __FUNCTION__, score);
    return score;
}
