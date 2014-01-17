/*
 * mobile_music_info_t.h
 *
 *
 *  Created on: 2013-6-26
 *      Author: zhengxie
 */

#ifndef MUSIC_INFO_T_H
#define MUSIC_INFO_T_H

#include "../output_type/common_header.h"
#include "../module/mb_music_content.h"

class mobile_music_info_t
{
public:
	static void set_outparameter(idl::mobile_music_info_t* out,
									 MCPMBMusicInfo* music_info);
};

#endif /* MOBILE_MUSIC_INFO_T */
