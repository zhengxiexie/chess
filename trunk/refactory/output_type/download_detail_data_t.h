/*
 * download_data_t.h
 *
 *  Created on: 2013-6-14
 *      Author: guoweijiang
 */

#ifndef DOWNLOAD_DATA_T_H_
#define DOWNLOAD_DATA_T_H_

#include "common_header.h"
#include "../module/mcp_content_data.h"

/*
struct download_detail_t
{
    string id;
    string url;
    string baidu_url;
    string url_type;
    string media_format;
    string size;
    string mtype;
    string charge_id;
    string charge_value;
    string apk_flag; //add by zhengxie
};
*/

class download_detail_data_t
{
public:
    static void set_outparameter(idl::download_detail_t* out,
                                 MCPContentDataInfo* download_data);
};

#endif /* DOWNLOAD_DATA_T_H_ */
