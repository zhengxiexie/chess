/*
 * notify_rpc.cpp
 *
 *  Created on: 2013-5-23
 *      Author: guoweijiang
 */

#include "notice_rpc.h"

NoticeRPC::NoticeRPC()
{
    this->init();
}

int NoticeRPC::init()
{
    comcfg::Configure conf;

    if (0 != conf.load("./conf", "notice_rpc.conf"))
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

int NoticeRPC::game_rpc(const char* user_id,
                        const char* content_text)
{
    LOGN("[GWJ] %s: start", __FUNCTION__);

    ubrpc::NonblockClient client(&_mgr);
    ubrpc::Client client1(&_mgr);
    bsl::syspool pool;

    int res(0);
    idl::Notify_addSystemNotify_params pin(&pool);
    idl::Notify_addSystemNotify_response pout(&pool);

    pin.set_sourceId("8");
    pin.set_titleId("10007");

    int u_id = atoi(user_id);
    pin.set_targetUid(0, u_id);
    pin.set_messageContent(content_text);
    pin.set_bussiness(8);

    client.call(&client1, "Notify", "addSystemNotify", &pin, &pout);
    client.waitAll();

    res = client1.getError();

    if(0 != res)
    {
        LOGN("[GWJ] %s: start. Notice Failed!!! [u_id:%s],[context:%s]. Error[%s]",
                __FUNCTION__, user_id, content_text, client1.getErrorMessage());

        return -1;
    }

    bool result = pout.result_params().resault();

    LOGN("[GWJ] %s: end[result:%d]", __FUNCTION__, result);
    return 0;
}

int NoticeRPC::notice_grab(const char* user_id,
                           const char* game_name,
                           const char* grab_number,
						   const char* push_channelid,
						   const char* push_userid)
{
    LOGN("[GWJ] %s: start. [u_id:%s],[game:%s],[grab:%s]",
         __FUNCTION__, user_id, game_name, grab_number);

    string content = string("name:") + game_name + ",type:20002,code:" + grab_number + ",pushchannelid:" + push_channelid + ",pushuserid:" + push_userid;

    return this->game_rpc(user_id, content.c_str());
}
