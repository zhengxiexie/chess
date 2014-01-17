/*
 * game_basic_info.h
 *
 *  Created on: 2013-6-13
 *      Author: guoweijiang
 */

#ifndef GAME_BASIC_INFO_H_
#define GAME_BASIC_INFO_H_

#include "common_header.h"
#include "../module/mcp_game_content.h"
#include "../input_type/content_input.h"

/*
struct content_basic_info_t
{
        string id;
        string name;
        string type;
        class_info_t class_info[];
        img_info_t img_info[];
        string ext_id;
        string abstract_;
        string version;
        string method;
        string star;
        string network;
        string code;
        string log_date;
        string mod_date;
        string dft_charge_type;
        string dft_charge_val;
        string enName;
        string language;
        string adapt;
        string package_name;
        string version_appinfo;
        string sdk_version;
        string version_code;
        string ptype_0;
        string ptype_1;
        string ptype_2;
        string ptype_3;
        string ptype_4;
        string pub_props[];

        cp_info_t cp_info;
        grant_detail_t grant_info;
};
 */
class game_basic_info_param
{
public:

    static void set_outparameter(idl::content_basic_info_t* out,
                                 content_input_params& in,
                                 MCPGameInfo* game_data);
};

#endif /* GAME_BASIC_INFO_H_ */
