/*
 * notify_rpc.h
 *
 *  Created on: 2013-5-23
 *      Author: guoweijiang
 */

#ifndef NOTIFY_RPC_H_
#define NOTIFY_RPC_H_

#include <bsl/var/JsonSerializer.h>
#include <bsl/var/JsonDeserializer.h>
#include "Configure.h"
#include "comlogplugin.h"
#include "../../mcp.idl.mcp.h"
#include "ubrpc/client/client.h"
#include "../common.h"

using namespace std;

class NoticeRPC
{
    NSP_SINGLETONIZE(NoticeRPC)

    ub::UbClientManager _mgr;

    ~NoticeRPC()
    {
        _mgr.close();
    }

    int init();
    int game_rpc(const char* user_id,
                 const char* content_text);

public:

    int notice_grab(const char* user_id,
                    const char* game_name,
					const char* grab_number,
					const char* push_channelid,
					const char* push_userid);
};

#endif /* NOTIFY_RPC_H_ */
