/*
 * big_tester.h
 *
 *  Created on: 2013-5-8
 *      Author: zhengxie
 */

#ifndef BIG_TESTER_H_
#define BIG_TESTER_H_

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
#include "../decorator/single_decorator/is_decorator.h"
#include "../decorator/single_decorator/or_decorator.h"
#include "../decorator/single_decorator/orderby_decorator.h"
#include "../decorator/single_decorator/weight_decorator.h"
#include "../decorator/single_decorator/bigger_decorator.h"
#include "../decorator/compound_decorator/decorator_for_test.h"
#include "../mysql_module/mysql_general_module.h"
#include "../output_type/grab_info_t.h"
#include "../output_type/game_basic_download_info.h"

#include "../../mcp_session.h"
#include "../../mcp.idl.mcp.h"
#include "../../mcpsvr.h"

void mysql_test2();
int test_fun_2();

#endif /* LITTEL_TESTER_H_ */
