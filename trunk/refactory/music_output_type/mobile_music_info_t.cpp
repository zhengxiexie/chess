/*
 * mobile_music_info_t.cpp
 *
 *  Created on: 2013-6-26
 *      Author: zhengxie
 */
#include "mobile_music_info_t.h"

void mobile_music_info_t::set_outparameter(idl::mobile_music_info_t* out,
									 MCPMBMusicInfo* music_info)
{
    LOGD("[GWJ] %s:  start", __FUNCTION__);

	if(!music_info)
	{
		return ;
	}
    out->set_ring_id(music_info->ring_id.c_str());
    out->set_name(music_info->name.c_str());
    out->set_singer(music_info->singer.c_str());
    out->set_bell_id(music_info->bell_id.c_str());
    out->set_song_id(music_info->song_id.c_str());
    out->set_cp_id(music_info->cp_id.c_str());
    out->set_date(music_info->date.c_str());
    out->set_dolby_flag(music_info->dolby_flag.c_str());
    out->set_status(music_info->status.c_str());
}
