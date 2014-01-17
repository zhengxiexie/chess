#ifndef _CONTENT_COMMON_H_
#define _CONTENT_COMMON_H_

#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include "mongo.h"
#include "mongo_connpool.h"
using namespace std;

extern mongo_connpool* mongo_pool;

namespace content
{

const string MCP_CONTENT_GAME( "1" );
const string MCP_CONTENT_THEME( "5" );
const string MCP_ANDROID_GAME( "11" );
const string MCP_NQ_GAME( "14" );
const string MCP_CONTENT_SOFT( "15" );
const string MCP_ANDROID_THEME( "16" );
const string MCP_IOS_GAME( "22" );
const string MCP_IOS_SOFT( "23" );
const string MCP_GAME_NEWS( "24" );
const string MCP_ONLINE_GAME( "25" );
const string MCP_H5_GAME( "26" );
const string MCP_H5_MUSIC( "29" );

const string MCP_MONGODB_NAME     ("mcp"                     );
const string MCP_MONGO_CONTENT_COL("mcp_content"             );
const string MCP_MONGO_GAME_COL   ("mcp_game_content"        );
const string MCP_MONGO_FORMAT_COL ("mcp_content_format"      );
const string MCP_MONGO_CLASS_COL  ("mcp_content_class"       );
const string MCP_MONGO_DATA_COL   ("mcp_content_data"        );
const string MCP_MONGO_PREVIEW_COL("mcp_content_preview"     );
const string MCP_MONGO_RANK_COL   ("mcp_content_rank"        );
const string MCP_MONGO_COLUMN_COL ("mcp_content_column_list" );
const string MCP_MONGO_DIY_COL    ("mcp_content_diy_column"  );
const string MCP_MONGO_THEME_COL  ("mcp_theme_content"       );
const string MCP_MONGO_CHARGE_COL ("mcp_content_chargetype"  );
const string MCP_MONGO_URL_COL    ("mcp_url_content"         );
const string MCP_MONGO_ADAPT_COL  ("mcp_class_adapt_count"   );
const string MCP_MONGO_LABEL_COL  ("mcp_content_bookmark"    );
const string MCP_MONGO_CP_COL     ("mcp_content_cp"          );
const string MCP_MONGO_GRANT_COL  ("mcp_content_grant"       );
const string MCP_MONGO_SOFT_COL   ("mcp_software_content"    );
const string MCP_ANDROID_THEME_COL("mcp_android_theme"       );
const string MCP_IOS_CONTENT_COL  ("mcp_ios_content"         );
const string MCP_MONGO_NEWS_COL   ("mcp_content_news"        );
const string MCP_MONGO_SCORE_COL  ("mcp_content_appscore"    );
const string MCP_APPINFO_COL      ("mcp_content_appinfo"     );
const string MCP_ONLINE_GAME_COL  ("mcp_online_game"         );
const string MCP_MIXED_TOPIC_MUSIC_COL ("mcp_mixed_topic_music");
const string MCP_RECENTLY_PLAYED_COL   ("mcp_content_recently_played");
const string MCP_FAVORITE_COL          ("mcp_content_favorite");
const string MCP_MONGO_CALENDER        ("mcp_content_calender");
const string MCP_MONGO_H5_MUSIC        ("mcp_content_H5_music");

struct img_info_t
{
	string type;
	string url;
	void clear()
	{
		type.clear();
		url.clear();
	}
};

struct class_info_t
{
	string id;
	string name;
	string icon;
    string type;
    int count;
	void clear()
	{
		id.clear();
		name.clear();
		icon.clear();
        type.clear();
        count=0;
	}
};

struct column_info_t
{
	string id;
	string name;
	void clear()
	{
		id.clear();
		name.clear();
	}
};

struct content_attribute_t
{
    int type;
    string content;
    void clear()
    {
        type = 0;
        content.clear();
    }
};

struct grant_detail_t
{
	string id;
	string grant_time;
	string valid_time;
	string modify_time;
	void clear()
	{
		id.clear();
		grant_time.clear();
		valid_time.clear();
		modify_time.clear();
	}
};

struct topic_info_t
{
	string id;
	string name;
	string type_id;
	string img;
	string intro;
	string log_date;
	string before;
	string next;
	void clear()
	{
		id.clear();
		name.clear();
		type_id.clear();
		img.clear();
		intro.clear();
		log_date.clear();
		before.clear();
		next.clear();
	}
};

struct soft_info_t
{
	string id;
	string name;
	string icon;
	string preview;
	string version;
	string size;
	string adapt;
	string date;
	string pop;
	string froms;
	string info;
	string down_url;
	string jump_url;
	vector<img_info_t> img_info;
	vector<class_info_t> class_info_list;
    string star;
	string show_highspeed;
	void clear()
	{
		id.clear();
		name.clear();
		icon.clear();
		preview.clear();
		version.clear();
		size.clear();
		adapt.clear();
		date.clear();
		pop.clear();
		froms.clear();
		info.clear();
		down_url.clear();
		img_info.clear();
		class_info_list.clear();
        star.clear();
		show_highspeed.clear();
	}
};

struct android_theme_t
{
	string id;
	string name;
	string icon;
	string preview;
	string size;
    string version;
	string adapt;
	string date;
	string pop;
	string froms;
	string info;
	string down_url;
	string jump_url;
	vector<img_info_t> img_info;
	vector<class_info_t> class_info_list;
	void clear()
	{
		id.clear();
		name.clear();
		icon.clear();
		preview.clear();
		size.clear();
		adapt.clear();
		date.clear();
		pop.clear();
		froms.clear();
		info.clear();
		down_url.clear();
		img_info.clear();
		class_info_list.clear();
        version.clear();
	}
};

struct ios_content_t{
	string id;
	string name;
	string type;
	string itunes_id;
	string info;
	string version;
	string cp_id;
	string star;
	string gdate;
    string mdate;
	string adapt;
	string d_id;
	string url;
	string jump_url;
	string size;
	string icon;
	string language;
    string thumb;
	double value;
	double original_value;
	int rating;
	int count;
	int crt_rating;
	int crt_count;
	vector<class_info_t> class_list;
	vector<string> img_list;
	void clear()
	{
		id.clear();
		name.clear();
		type.clear();
		itunes_id.clear();
		info.clear();
		version.clear();
		cp_id.clear();
		star.clear();
		gdate.clear();
		mdate.clear();
		adapt.clear();
		d_id.clear();
		url.clear();
		jump_url.clear();
		size.clear();
		icon.clear();
        language.clear();
		class_list.clear();
		img_list.clear();
        thumb.clear();
		value=0;
		original_value=0;
		rating=0;
		count=0;
		crt_rating=0;
		crt_count=0;
	}
};

struct game_brief_t{
	string info_type;
	string info_id;
	string info_title;
	string info_datetime;
    string game_type;
	void clear()
	{
		info_type.clear();
		info_id.clear();
		info_title.clear();
		info_datetime.clear();
        game_type.clear();
	}
};

struct game_t{
	string info_type;
	string info_id;
	string c_id;
	string info_title;
	string info_content;
	string info_date;
    string game_type;
	void clear()
	{
		info_type.clear();
		info_id.clear();
		c_id.clear();
		info_title.clear();
		info_content.clear();
		info_date.clear();
        game_type.clear();
	}
};

struct game_score_t{
	string id;
	string c_id;
	string type;
	string score;
	void clear()
	{
		id.clear();
		c_id.clear();
		type.clear();
		score.clear();
	}
};

struct appinfo_t{
	string id;
	string c_id;
	string app_name;
	string package_name;
	string version_code;
	string version;
	string sdk_version;
	string permissions;
	string features;
	string apk_info;
	void clear()
	{
		id.clear();
		c_id.clear();
		app_name.clear();
		package_name.clear();
		version_code.clear();
		version.clear();
		sdk_version.clear();
		permissions.clear();
		features.clear();
		apk_info.clear();
	}
};

struct online_game_t
{
    string id;
    string name;
    string icon;
    string thumb;
    string type;
    string info;
    string version;
    string system_req;
    string cp_id;
    string cp_name;
    double star;
    int popularity;
    string language;
    string gdate;
    string mdate;
    string ldate;
    string url;
    string apk_flag; //add by zhengxie
    int size;
    vector<string> label_list;
    vector<string> img_list;
    vector<class_info_t> class_list;
    vector<content_attribute_t> attribute_list;
    
    // [s] add by gwj for mcp_content_pub_prop
    string pub_prop_ids;
    // [e] add by gwj for mcp_content_pub_prop
	// [s] add by zhengxie for editor_recom
    string editor_recom;
	// [e] add by zhengxie for editor_recom

    void clear()
    {
        id.clear();
        name.clear();
        icon.clear();
        thumb.clear();
        type.clear();
        info.clear();
        version.clear();
        system_req.clear();
        cp_id.clear();
        cp_name.clear();
        star = 0.0;
        popularity = 0;
        language.clear();
        gdate.clear();
        mdate.clear();
        ldate.clear();
        url.clear();
        size = 0;
        label_list.clear();
        img_list.clear();
        class_list.clear();
        attribute_list.clear();
        
        // [s] add by gwj for mcp_content_pub_prop
        pub_prop_ids.clear();
        // [e] add by gwj for mcp_content_pub_prop
    }
};

struct grab_t{
    uint32_t id;
    string name;
    string info;
    string c_id;
    uint32_t amount;
    uint32_t send_amount;
    string start_time;
    string end_time;
    double probability;
    grab_t(){
        id = 0;
        amount = 0;
        send_amount = 0;
    }
    void clear()
    {
        id=0;
        name.clear();
        info.clear();
        c_id.clear();
        amount=0;
        send_amount=0;
        start_time.clear();
        end_time.clear();
        probability = 0.0;
    }
    grab_t& operator = (const grab_t& grab_info)
    {
        if( this == &grab_info ){
            return *this;
        }
        this->id = grab_info.id;
        this->name = grab_info.name;
        this->info = grab_info.info;
        this->c_id = grab_info.c_id;
        this->amount = grab_info.amount;
        this->send_amount = grab_info.send_amount;
        this->start_time = grab_info.start_time;
        this->end_time = grab_info.end_time;
        this->probability = grab_info.probability;
        return *this;
    }
};

struct calender_t{
    uint32_t id;
    string title;
    uint32_t c_id;
    uint32_t type;
    string enable_date;
    void clear()
    {
        id=0;
        title.clear();
        c_id = 0;
        type = 0;
        enable_date.clear();
    }
};

struct music_t{
	string type_id;
	string music_id;
	string songname;
	string area_id;
	string area_name;
	string cp_id;
	string cp_name;
	string language_id;
	string language;
	string publish_date;
	string intro;
	string lyric_writer;
	string composer;
	string price;
	string discount;
	string download_base;
	string lyric_txt;
	void clear()
	{
		type_id.clear();
		music_id.clear();
		songname.clear();
		area_id.clear();
		area_name.clear();
		cp_id.clear();
		cp_name.clear();
		language_id.clear();
		language.clear();
		publish_date.clear();
		intro.clear();
		lyric_writer.clear();
		composer.clear();
		price.clear();
		discount.clear();
		download_base.clear();
		lyric_txt.clear();
	}
};

struct duoku_paradise_game_t
{
	string id;
	string name;
	string icon_url;
	string img_small_url;
	string img_big_url;
	string num_playing;
	string class_id;
	string class_name;
	string intro;
	void clear()
	{
		id.clear();
		name.clear();
		icon_url.clear();
		img_small_url.clear();
		img_big_url.clear();
		num_playing.clear();
		class_id.clear();
		class_name.clear();
		intro.clear();
	}
};

struct img_h5_url_t
{
	string icon_url;
	string xc1_url;
	string xc2_url;
	vector<string> xq_url_list;
	void clear()
	{
		icon_url.clear();
		xc1_url.clear();
		xc2_url.clear();
		xq_url_list.clear();
	}
};

template <typename T>
void getClassInfoFromBson(bson_iterator* it, T& info);
template <typename T>
void getImgInfoFromBson(bson_iterator* bson_it, T& info, const string& icon_type, const string preview_type = "");
template <typename T>
void getLabelFromBson(bson_iterator* bson_it, T& info);
template <typename T>
void getAttributeFromBson(bson_iterator* bson_it, T& info);

int get_ios_content_from_bson(const bson* result, ios_content_t& ios_content );
int get_game_info_from_bson(const bson* result, game_t& game_info);
int get_game_brief_info_from_bson(const bson* result, game_brief_t& game_brief_info);
int get_game_score_from_bson(const bson*, content::game_score_t&);
int get_appinfo_from_bson(const bson*, content::appinfo_t&);
int get_cid_from_bson(const bson* result, string& cid);

void free_data( mongo_cursor * cursor, bson* query, bson* fields, bson* bson_result );

int getCalenderList(vector<calender_t>& list);

online_game_t getOnlineGameFromBson(const bson* result);
int getOnlineGameFromBson(const bson* result, online_game_t& info);

class mcp_content_map
{
	public:
		static int init();
		static int get_class_info(const string& id, class_info_t& class_info);
		static int get_column_info(const string& id, column_info_t& column_info);
		static int get_label_name(const string& id, string& name);
		static string get_label_name(const string& id);
		static int get_charge_name(const string& id, string& name);
		static int get_cp_name(const string& id, string& name);
		static int get_grant_info(const string& id, grant_detail_t& grant_info);
		static int get_topic_info(const string& id, topic_info_t& topic_info);
		static int clear();
		static string convert_mongo_ts_to_string(bson_iterator* it);
		static string get_attribute_name(const int id);

		static map<string, vector<string> > m_type_topic_list;

		static int get_class_list_by_type(const string& type_id, vector<class_info_t>& class_list);

	protected:
	private:
		static map<string,class_info_t> m_class_id_info;
		static map<string,column_info_t> m_column_id_info;
		static map<string,string> m_label_id_name;
		static map<string,string> m_charge_id_name;
		static map<string,string> m_cp_id_name;
		static map<string,grant_detail_t> m_grant_id_info;
		static map<string,topic_info_t> m_topic_id_info;
		static map<int,string> m_attr_id_name;;

		static int init_class_id_info();
		static int init_column_id_info();
		static int init_label_id_name();
		static int init_charge_id_name();
		static int init_cp_id_name();
		static int init_grant_id_info();
		static int init_topic_id_info();
		static int init_attribute_name();
};

class commonFunc
{
	public:
		/*
		 *生成0到n-1的随机序列
		 *
		 */
		static vector<int> randomIntSequence(const int n);
        static int get_typeid_by_colid(const string col_id, string& type_id);
};

};

#endif
