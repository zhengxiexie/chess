/*
 * mb_music_content.h
 *
 *  专门负责操作  MongoDB 中  mcp.mb_music_content 表的类
 *
 *  Created on: 2013-6-27
 *      Author: zhengxie
 */

#ifndef MB_MUSIC_CONTENT_H_
#define MB_MUSIC_CONTENT_H_

#include "abstract_mcp_content.h"

class MCPMBMusicInfo : public AbstractMCPRecord
{
public:

    static const char* ClassName;

    string ring_id;
    string name;
    string singer;
    string bell_id;
    string song_id;
    string cp_id;
    string date;
    string dolby_flag;
    string status;

    void clear()
    {
        LOGG("[GWJ] %s:  start ", __FUNCTION__);
        ring_id.clear();
        name.clear();
        singer.clear();
        bell_id.clear();
        song_id.clear();
        cp_id.clear();
        date.clear();
        dolby_flag.clear();
        status.clear();
    }

    MCPMBMusicInfo():AbstractMCPRecord()
    {
    }

    virtual ~MCPMBMusicInfo()
    {
        this->clear();
    }
};

class MCPMBMusicContent : public AbstractMCPContent
{
protected:

    int set_basic_info_from_bson_result(bson* bson_out,
                                        const char* aggregate_name);

public:
    MCPMBMusicContent(const char* key_name);
    virtual ~MCPMBMusicContent();
};

#endif /* MB_MUSIC_CONTENT_H_ */
