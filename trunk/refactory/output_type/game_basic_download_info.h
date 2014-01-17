/*
 * game_basic_download_info.h
 *
 *  Created on: 2013-5-9
 *      Author: guoweijiang
 */

#ifndef GAME_BASIC_DOWNLOAD_INFO_H_
#define GAME_BASIC_DOWNLOAD_INFO_H_

#include "common_header.h"
#include "../module/mcp_content_data.h"
#include "../module/mcp_game_content.h"
#include "../input_type/content_input.h"
#include "download_detail_data_t.h"

class game_basic_download_t
{
public:

    static void set_outparameter(idl::content_detail_t* out,
                                 content_input_params& in,
                                 TwoBindedRecord* binded_record);
};

#endif /* GAME_BASIC_DOWNLOAD_INFO_H_ */
