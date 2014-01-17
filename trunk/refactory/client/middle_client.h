/*
 * middle_client.h
 *
 *  Created on: 2013-09-04
 *      Author: zhengxie
 */

#ifndef MIDDLE_CLIENT_H_
#define MIDDLE_CLIENT_H_

#include <bsl/var/JsonSerializer.h>
#include <bsl/var/JsonDeserializer.h>
#include "Configure.h"
#include "comlogplugin.h"
#include "../../mcp.idl.mcp.h"
#include "ubrpc/client/client.h"
#include "../common.h"

using namespace std;

class MiddleClient
{
    NSP_SINGLETONIZE(MiddleClient)

    ub::UbClientManager _mgr;

    ~MiddleClient()
    {
        _mgr.close();
    }

    int init();

public:

	int addGameHallScore(uint32_t userId, uint32_t opId); 
};

#endif /* MIDDLE_CLIENT_H_ */
