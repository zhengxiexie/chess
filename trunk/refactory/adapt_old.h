/*
 * adapt_old.h
 *
 *  Created on: 2013-8-27
 *      Author: guoweijiang
 */

#ifndef ADAPT_OLD_H_
#define ADAPT_OLD_H_

#include "module/mcp_content_data.h"
#include "module/mcp_game_content.h"
#include "module/mcp_content_news.h"
#include "module/mcp_content_common_info.h"
#include "module/mcp_content_diy_column.h"
#include "module/mcp_netgame_activity.h"
#include "module/mcp_diff_update_info.h"
#include "decorator/single_decorator/is_decorator.h"
#include "decorator/single_decorator/or_decorator.h"
#include "decorator/single_decorator/orderby_decorator.h"
#include "decorator/single_decorator/weight_decorator.h"
#include "decorator/single_decorator/bigger_decorator.h"
#include "decorator/single_decorator/smaller_decorator.h"
#include "decorator/single_decorator/not_equal_decorator.h"
#include "decorator/compound_decorator/decorator_for_test.h"
#include "output_type/once_consume_goods.h"
#include "output_type/game_basic_download_info.h"
#include "output_type/game_info_t.h"
#include "output_type/game_news_info_t.h"
#include "output_type/column_info_t.h"
#include "output_type/netgame_activity_info_t.h"
#include "output_type/game_update_download_info.h"
#include "output_type/game_content_download_info_t.h"
#include "output_type/game_news_entire_info_t.h"
#include "input_type/game_news_info_input.h"
#include "input_type/content_input.h"
#include "input_type/input_extractor.h"
#include "input_type/json_extractor.h"
#include "redis_module/duoku_user_download.h"
#include "redis_module/device_download.h"
#include "redis_module/duoku_user_guide.h"
#include "redis_module/duoku_user_favorite.h"
#include "mysql_module/mysql_general_module.h"

/*
 *  一个专门为处理老接口使用的类
 *  老接口可以通过这个 class 调用新接口中的重构框架
 */
class AdaptOldMcpInterface
{
public:

    /*
     *  判断 c_id 是否在 [mcp_content] 表中存在
     */
    static bool is_enable_game(const char* c_id);
};


#endif /* ADAPT_OLD_H_ */
