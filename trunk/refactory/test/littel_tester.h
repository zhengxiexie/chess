/*
 * littel_tester.h
 *
 *  Created on: 2013-5-8
 *      Author: guoweijiang
 */

#ifndef LITTEL_TESTER_H_
#define LITTEL_TESTER_H_

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <map>
#include <time.h>

#include "../common.h"
#include "../module/mcp_content_data.h"
#include "../module/mcp_game_content.h"
#include "../module/mcp_content_common_info.h"
#include "../module/mcp_content_news.h"
#include "../decorator/single_decorator/is_decorator.h"
#include "../decorator/single_decorator/or_decorator.h"
#include "../decorator/single_decorator/orderby_decorator.h"
#include "../decorator/single_decorator/weight_decorator.h"
#include "../decorator/single_decorator/bigger_decorator.h"
#include "../decorator/compound_decorator/decorator_for_test.h"
#include "../mysql_module/mysql_general_module.h"
#include "../output_type/once_consume_goods.h"
#include "../output_type/game_basic_download_info.h"
#include "../rpc/notice_rpc.h"
#include "../common_simple_buf.h"
#include "../redis_module/redis_module.h"

#include "../../mcp_session.h"
#include "../../mcp.idl.mcp.h"
#include "../../mcpsvr.h"

#include "../../grab.h"

void mysql_test();
int test_fun();
void test_upmongodb();

#endif /* LITTEL_TESTER_H_ */
