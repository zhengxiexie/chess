#ifndef MCPSVR_H
#define MCPSVR_H

#include <iostream>
#include <algorithm>
#include <stdio.h>
#include <string>
#include <map>
#include <vector>
#include <time.h>
#include "myclient_include.h"
#include "mcp.idl.mcp.h"
#include "mcp_session.h"
#include "mongo.h"
using namespace std;

typedef struct Data
{
string numseg;
string type;
string oid;
string province;
string city;
string code;
string enabled;
};



extern MyclientPool * database_pool;
extern mongo conn[1];
extern char MCP_MONGODB_HOST[2][16];
extern char MCP_MONGODB_PORT[2][6];
extern map<int,struct Data *> database_map;
extern map<int,int> invalid_flag;
extern int mcpupdate_flag;
extern const int phonevalid_update_flag;
typedef enum
{
    PHONE_MAKER = 0,
    PHONE_PLAT
}info_type;

struct mcp_brand_st
{
    char id[12];
    char name[201];
    char phone_count[32];
    mcp_brand_st()
    {
        memset(id, 0x00, sizeof(id));
        memset(name, 0x00, sizeof(name));
        memset(phone_count, 0x00, sizeof(phone_count));
    }
};

struct mcp_content_list_st
{
    char content_id[12];
    char content_name[101];
    char image_url[256];
    char download_id[12];
    char download_url[256];
    char charge_id[12];
    char charge_type_id[12];
};


#define    MCP_OK  0
#define    MCP_NG  -1

#define    PROBABLY_LIKE_TYPE_CP    1
#define    PROBABLY_LIKE_TYPE_CLASS 2
#define    PROBABLY_LIKE_TYPE_BOTH  3

int mcp_connect_mongo_hosts( mongo* &conn );
int check_mongo_conn( mongo* conn );

int get_cdata_id_by_type( mongo* conn, const string content_id, const string info_type,
                                       const string info_id, vector<string>& cdata_id_list );

class McpServlet : public idl::mcp_servlet_trunk
{

public:
    McpServlet::McpServlet(){ }
    McpServlet::~McpServlet(){ }
	int get_phone_area_by_phone_num(const idl::mcp_get_phone_area_by_phone_num_params &in,idl::mcp_get_phone_area_by_phone_num_response &out);
     int get_test(const idl::mcp_get_test_params &in, idl::mcp_get_test_response &out) ;
    int get_phone_maker_plat(const idl::mcp_get_phone_maker_plat_params& in, idl::mcp_get_phone_maker_plat_response& out);
    //通过类别id取得资源列表信息
    int get_content_list_from_classID(const idl::mcp_get_content_list_from_classID_params& in,
                                                        idl::mcp_get_content_list_from_classID_response& out);
    //通过类型id取得资源列表信息
    int get_content_list_from_typeID(const idl::mcp_get_content_list_from_typeID_params& in,
                                                        idl::mcp_get_content_list_from_typeID_response& out);
    //根据传入资源类型ID，返回对应的类别ID和类别名称
    int get_content_class(const idl::mcp_get_content_class_params& in, idl::mcp_get_content_class_response& out);
    //根据类别id，取得类别名称
    int get_content_class_name(const idl::mcp_get_content_class_name_params& in, idl::mcp_get_content_class_name_response& out);
    int get_content_count_from_class(const idl::mcp_get_content_count_from_class_params& in,
                                           idl::mcp_get_content_count_from_class_response& out);
    int get_content_diy_col_name(const idl::mcp_get_content_diy_col_name_params& in, idl::mcp_get_content_diy_col_name_response& out);
    //根据传入的ID信息，返回对应的内容资源信息
    int get_content_info(const idl::mcp_get_content_info_params& in, idl::mcp_get_content_info_response& out);
    //根据资源id，返回对应的下载信息
    int get_download_info(const idl::mcp_get_download_info_params& in, idl::mcp_get_download_info_response& out);
    //根据指定的优先级，返回资源信息列表
    int get_content_list(const idl::mcp_get_content_list_params& in, idl::mcp_get_content_list_response& out);
    //根据指定的栏目id，返回资源信息列表
    int get_content_list_from_part(const idl::mcp_get_content_list_from_part_params& in, idl::mcp_get_content_list_from_part_response& out);
    //根据传入的User Agent或Pid（机型ID）信息与库里的UA进行匹配，返回对应的手机基本类型信息
    int get_phone_info(const idl::mcp_get_phone_info_params& in, idl::mcp_get_phone_info_response& out);
    //根据手机的厂商/平台ID，返回所属厂商的手机机型信息
    int get_phone_type_from_ID(const idl::mcp_get_phone_type_from_ID_params& in, idl::mcp_get_phone_type_from_ID_response& out);
    //根据手机的厂商ID，返回所属厂商的手机机型首字母列表
    int get_phone_headword_from_maker(const idl::mcp_get_phone_headword_from_maker_params& in, idl::mcp_get_phone_headword_from_maker_response& out);
    int search_phone_info(const idl::mcp_search_phone_info_params& in, idl::mcp_search_phone_info_response& out);
    //根据ip地址，返回所属运营商，省份信息
    int get_carriers_info_from_ip(const idl::mcp_get_carriers_info_from_ip_params& in, idl::mcp_get_carriers_info_from_ip_response& out);
    //根据手机的厂商/平台ID，返回所属厂商的手机机型信息
    int get_phone_info_from_maker_plat(const idl::mcp_get_phone_info_from_maker_plat_params& in, idl::mcp_get_phone_info_from_maker_plat_response& out);
    int get_mcp_stat(const idl::mcp_get_mcp_stat_params& in, idl::mcp_get_mcp_stat_response& out);
    int get_url_info_by_id(const idl::mcp_get_url_info_by_id_params& in, idl::mcp_get_url_info_by_id_response& out);
    int get_url_info_by_class_id(const idl::mcp_get_url_info_by_class_id_params& in, idl::mcp_get_url_info_by_class_id_response& out);
    int get_url_info_by_column_id(const idl::mcp_get_url_info_by_column_id_params& in, idl::mcp_get_url_info_by_column_id_response& out);

    int get_phone_uaJson(const idl::mcp_get_phone_uaJson_params& in, idl::mcp_get_phone_uaJson_response& out);
    int init_phone_uaJson_map(const idl::mcp_init_phone_uaJson_map_params& in, idl::mcp_init_phone_uaJson_map_response& out);

    int get_content_list_from_phone(const idl::mcp_get_content_list_from_phone_params &,
                                          idl::mcp_get_content_list_from_phone_response &);

    int get_content_list_by_rank(const idl::mcp_get_content_list_by_rank_params& in,
                                       idl::mcp_get_content_list_by_rank_response& out);

    int get_label_by_content_id(const idl::mcp_get_label_by_content_id_params& in,
                                      idl::mcp_get_label_by_content_id_response& out);

    int get_content_list_by_label(const idl::mcp_get_content_list_by_label_params& in,
                                        idl::mcp_get_content_list_by_label_response& out);

    int get_topic_info(const idl::mcp_get_topic_info_params& in, idl::mcp_get_topic_info_response& out);

    int get_content_count_by_class(const idl::mcp_get_content_count_by_class_params& in,
                                         idl::mcp_get_content_count_by_class_response& out);
    int get_content_img(const idl::mcp_get_content_img_params& in, idl::mcp_get_content_img_response& out);

    int get_label_name_by_id(const idl::mcp_get_label_name_by_id_params& in, idl::mcp_get_label_name_by_id_response& out);

    int get_content_list_by_date(const idl::mcp_get_content_list_by_date_params& in,
                                       idl::mcp_get_content_list_by_date_response& out);

    int get_soft_info_by_id(const idl::mcp_get_soft_info_by_id_params& in, idl::mcp_get_soft_info_by_id_response& out);

    int get_col_list_by_topic(const idl::mcp_get_col_list_by_topic_params& in, idl::mcp_get_col_list_by_topic_response& out);

    int get_soft_list_by_col_id(const idl::mcp_get_soft_list_by_col_id_params& in, idl::mcp_get_soft_list_by_col_id_response& out);

    int get_soft_list_by_class_id(const idl::mcp_get_soft_list_by_class_id_params& in,
                                        idl::mcp_get_soft_list_by_class_id_response& out);

    int mcp_content_common_init(const idl::mcp_mcp_content_common_init_params& in, idl::mcp_mcp_content_common_init_response& out );

    int get_topic_list_by_type(const idl::mcp_get_topic_list_by_type_params& in, idl::mcp_get_topic_list_by_type_response& out);

    int get_android_theme_by_col_id(const idl::mcp_get_android_theme_by_col_id_params& in,
                                          idl::mcp_get_android_theme_by_col_id_response& out);

    int get_android_theme_by_class_id(const idl::mcp_get_android_theme_by_class_id_params& in,
                                            idl::mcp_get_android_theme_by_class_id_response& out);

    int get_android_theme_by_id(const idl::mcp_get_android_theme_by_id_params& in,
                                      idl::mcp_get_android_theme_by_id_response& out);

    int get_sub_class_by_class(const idl::mcp_get_sub_class_by_class_params& in, idl::mcp_get_sub_class_by_class_response& out);

    int reload_config_by_id(const idl::mcp_reload_config_by_id_params& in, idl::mcp_reload_config_by_id_response& out) ;

    int get_ios_content_by_id(const idl::mcp_get_ios_content_by_id_params& in, idl::mcp_get_ios_content_by_id_response& out);

    int get_ios_content_by_class_id(const idl::mcp_get_ios_content_by_class_id_params& in,
                                          idl::mcp_get_ios_content_by_class_id_response& out);

    int get_ios_content_by_cp_id(const idl::mcp_get_ios_content_by_cp_id_params& in,
                                       idl::mcp_get_ios_content_by_cp_id_response& out);

    int get_ios_content_by_col_id(const idl::mcp_get_ios_content_by_col_id_params& in,
                                        idl::mcp_get_ios_content_by_col_id_response& out);

    int get_ios_content_by_rank(const idl::mcp_get_ios_content_by_rank_params& in,
                                      idl::mcp_get_ios_content_by_rank_response& out);

    int get_class_list_by_type_id(const idl::mcp_get_class_list_by_type_id_params& in,
                                        idl::mcp_get_class_list_by_type_id_response &out);

	int get_ios_probably_like_list_by_id(const idl::mcp_get_ios_probably_like_list_by_id_params& in,
		        							   idl::mcp_get_ios_probably_like_list_by_id_response& out);

	int get_game_info_list(const idl::mcp_get_game_info_list_params& in,
								 idl::mcp_get_game_info_list_response& out);

	int get_game_info(const idl::mcp_get_game_info_params& in,
							idl::mcp_get_game_info_response& out);

    int get_game_info_list_by_c_id(const idl::mcp_get_game_info_list_by_c_id_params& in,
                                         idl::mcp_get_game_info_list_by_c_id_response& out);

	int get_game_score_by_imei_or_uid(const idl::mcp_get_game_score_by_imei_or_uid_params& in,
			                idl::mcp_get_game_score_by_imei_or_uid_response& out);

	int get_appinfo_by_packagename(const idl::mcp_get_appinfo_by_packagename_params& in,
			                idl::mcp_get_appinfo_by_packagename_response& out);

	int get_score_by_packagename_and_versioncode(const idl::mcp_get_score_by_packagename_and_versioncode_params& in,
			                idl::mcp_get_score_by_packagename_and_versioncode_response& out);

	int get_content_info_by_packagename_and_versioncode(const idl::mcp_get_content_info_by_packagename_and_versioncode_params& in,
		                idl::mcp_get_content_info_by_packagename_and_versioncode_response& out);

	int get_content_info_by_packagename(const idl::mcp_get_content_info_by_packagename_params& in,
			                idl::mcp_get_content_info_by_packagename_response& out);

    int get_online_game_by_id(const idl::mcp_get_online_game_by_id_params& in, idl::mcp_get_online_game_by_id_response& out);

    int get_online_game_by_class_id(const idl::mcp_get_online_game_by_class_id_params& in,
                                          idl::mcp_get_online_game_by_class_id_response& out);

    int get_online_game_by_cp_id(const idl::mcp_get_online_game_by_cp_id_params& in,
                                       idl::mcp_get_online_game_by_cp_id_response& out);

    int get_online_game_by_col_id(const idl::mcp_get_online_game_by_col_id_params& in,
                                        idl::mcp_get_online_game_by_col_id_response& out);

    int get_online_game_by_rank(const idl::mcp_get_online_game_by_rank_params& in, idl::mcp_get_online_game_by_rank_response& out);

    int get_grab_list_by_type(const idl::mcp_get_grab_list_by_type_params& in, idl::mcp_get_grab_list_by_type_response& out);

    int get_grab_info_by_c_id(const idl::mcp_get_grab_info_by_c_id_params& in, idl::mcp_get_grab_info_by_c_id_response& out);

    int get_grab_info_by_id(const idl::mcp_get_grab_info_by_id_params& in, idl::mcp_get_grab_info_by_id_response& out);

    int user_grab_num(const idl::mcp_user_grab_num_params& in, idl::mcp_user_grab_num_response& out);
    int user_grab_num_pre(const idl::mcp_user_grab_num_pre_params& in, idl::mcp_user_grab_num_pre_response& out);

	int get_user_grab_num_list(const idl::mcp_get_user_grab_num_list_params& in, idl::mcp_get_user_grab_num_list_response& out);

    int set_user_rating(const idl::mcp_set_user_rating_params& in, idl::mcp_set_user_rating_response& out);

    int get_user_rating(const idl::mcp_get_user_rating_params& in, idl::mcp_get_user_rating_response& out);

    int get_calender_list(const idl::mcp_get_calender_list_params& in, idl::mcp_get_calender_list_response& out);

	int get_content_list_by_mixed_topic_id(const idl::mcp_get_content_list_by_mixed_topic_id_params& in,
												 idl::mcp_get_content_list_by_mixed_topic_id_response& out);

    int McpServlet::get_content_list_by_mixed_topic_id1(const idl::mcp_get_content_list_by_mixed_topic_id1_params& in,
                            idl::mcp_get_content_list_by_mixed_topic_id1_response& out);

	int get_user_grab_num(const idl::mcp_get_user_grab_num_params &, idl::mcp_get_user_grab_num_response &);

	int set_recently_played(const idl::mcp_set_recently_played_params& in, idl::mcp_set_recently_played_response& out);

	int get_recently_played_by_uid(const idl::mcp_get_recently_played_by_uid_params& in, idl::mcp_get_recently_played_by_uid_response& out);

	int add_game_to_favorite(const idl::mcp_add_game_to_favorite_params& in, idl::mcp_add_game_to_favorite_response& out);

	int get_favoraite_by_uid(const idl::mcp_get_favoraite_by_uid_params& in, idl::mcp_get_favoraite_by_uid_response& out);

	int get_H5_latest_on_shelf_by_date(const idl::mcp_get_H5_latest_on_shelf_by_date_params& in,
									        idl::mcp_get_H5_latest_on_shelf_by_date_response& out);
	int get_H5_latest_on_shelf_by_date_and_type(const idl::mcp_get_H5_latest_on_shelf_by_date_and_type_params& in,
                idl::mcp_get_H5_latest_on_shelf_by_date_and_type_response& out);

	int get_H5_info_list_by_col_id(const idl::mcp_get_H5_info_list_by_col_id_params& in,
										idl::mcp_get_H5_info_list_by_col_id_response& out);

	int get_imgurls_and_favorite_by_uid_cid(const idl::mcp_get_imgurls_and_favorite_by_uid_cid_params& in,
													idl::mcp_get_imgurls_and_favorite_by_uid_cid_response& out);

	int McpServlet::get_random_h5_games(const idl::mcp_get_random_h5_games_params& in, idl::mcp_get_random_h5_games_response& out);

    int McpServlet::get_info_by_col_id_and_type_id(const idl::mcp_get_info_by_col_id_and_type_id_params& in,
                                    idl::mcp_get_info_by_col_id_and_type_id_response& out);

    int McpServlet::get_info_by_col_id(const idl::mcp_get_info_by_col_id_params& in,
                                    idl::mcp_get_info_by_col_id_response& out);

    int McpServlet::get_soft_by_date_or_star(const idl::mcp_get_soft_by_date_or_star_params& in,
                    idl::mcp_get_soft_by_date_or_star_response& out);

    int McpServlet::get_star_by_cid_and_type(const idl::mcp_get_star_by_cid_and_type_params& in,
                    idl::mcp_get_star_by_cid_and_type_response& out);

    int McpServlet::get_content_list_from_classID_with_weight(const idl::mcp_get_content_list_from_classID_with_weight_params& in,
                    idl::mcp_get_content_list_from_classID_with_weight_response& out);

    int McpServlet::get_content_with_download_info(const idl::mcp_get_content_with_download_info_params& in, idl::mcp_get_content_with_download_info_response& out);

    int McpServlet::get_once_consume_goods_list(const idl::mcp_get_once_consume_goods_list_params& in, idl::mcp_get_once_consume_goods_list_response& out);

    int McpServlet::get_game_info_list_ext(const idl::mcp_get_game_info_list_ext_params& in, idl::mcp_get_game_info_list_ext_response& out);
	int McpServlet::get_grab_list(const idl::mcp_get_grab_list_params& in, idl::mcp_get_grab_list_response& out);
	int McpServlet::get_common_adv_info(const idl::mcp_get_common_adv_info_params& in, idl::mcp_get_common_adv_info_response& out);
	int McpServlet::add_new_user_consume_record(const idl::mcp_add_new_user_consume_record_params& in, idl::mcp_add_new_user_consume_record_response& out);

    int McpServlet::add_new_download_game_by_uid(const idl::mcp_add_new_download_game_by_uid_params& in, idl::mcp_add_new_download_game_by_uid_response& out);
    int McpServlet::get_download_games_count_by_uid(const idl::mcp_get_download_games_count_by_uid_params& in, idl::mcp_get_download_games_count_by_uid_response& out);
    int McpServlet::get_download_games_info_by_uid(const idl::mcp_get_download_games_info_by_uid_params& in, idl::mcp_get_download_games_info_by_uid_response& out);
    int McpServlet::add_new_guide_by_uid(const idl::mcp_add_new_guide_by_uid_params& in, idl::mcp_add_new_guide_by_uid_response& out);
    int McpServlet::get_guides_count_by_uid(const idl::mcp_get_guides_count_by_uid_params& in, idl::mcp_get_guides_count_by_uid_response& out);
    int McpServlet::get_guides_info_by_uid(const idl::mcp_get_guides_info_by_uid_params& in, idl::mcp_get_guides_info_by_uid_response& out);
    int McpServlet::get_game_news_info_list(const idl::mcp_get_game_news_info_list_params& in, idl::mcp_get_game_news_info_list_response& out);
    int McpServlet::is_user_has_relation_with(const idl::mcp_is_user_has_relation_with_params& in, idl::mcp_is_user_has_relation_with_response& out);
    int McpServlet::add_new_favorite_by_uid(const idl::mcp_add_new_favorite_by_uid_params& in, idl::mcp_add_new_favorite_by_uid_response& out);
    int McpServlet::get_favorites_count_by_uid(const idl::mcp_get_favorites_count_by_uid_params& in, idl::mcp_get_favorites_count_by_uid_response& out);
    int McpServlet::get_favorites_info_by_uid(const idl::mcp_get_favorites_info_by_uid_params& in, idl::mcp_get_favorites_info_by_uid_response& out);
    int McpServlet::remove_favorite_by_uid(const idl::mcp_remove_favorite_by_uid_params& in, idl::mcp_remove_favorite_by_uid_response& out);
    int McpServlet::remove_guide_by_uid(const idl::mcp_remove_guide_by_uid_params& in, idl::mcp_remove_guide_by_uid_response& out);
    int McpServlet::get_game_contents_count(const idl::mcp_get_game_contents_count_params& in, idl::mcp_get_game_contents_count_response& out);
    int McpServlet::get_columns_info(const idl::mcp_get_columns_info_params& in, idl::mcp_get_columns_info_response& out);
    int McpServlet::get_netgame_activity_info(const idl::mcp_get_netgame_activity_info_params& in, idl::mcp_get_netgame_activity_info_response& out);
    int McpServlet::get_game_download_app_info(const idl::mcp_get_game_download_app_info_params& in, idl::mcp_get_game_download_app_info_response& out);
    int McpServlet::get_game_dbapp_detail_info(const idl::mcp_get_game_dbapp_detail_info_params& in, idl::mcp_get_game_dbapp_detail_info_response& out);
    int McpServlet::get_device_download_games_info_by_udid(const idl::mcp_get_device_download_games_info_by_udid_params& in, idl::mcp_get_device_download_games_info_by_udid_response& out);
    int McpServlet::add_device_download_game(const idl::mcp_add_device_download_game_params& in, idl::mcp_add_device_download_game_response& out);
    int McpServlet::get_game_content_info_list_ext(const idl::mcp_get_game_content_info_list_ext_params& in, idl::mcp_get_game_content_info_list_ext_response& out);
    int McpServlet::get_game_currency_rate(const idl::mcp_get_game_currency_rate_params& in, idl::mcp_get_game_currency_rate_response& out);
	  int McpServlet::get_game_download_info(const idl::mcp_get_game_download_info_params& in, idl::mcp_get_game_download_info_response& out);
	  int McpServlet::get_game_news_entire_info(const idl::mcp_get_game_news_entire_info_params& in, idl::mcp_get_game_news_entire_info_response& out);
	  int McpServlet::get_game_news_without_pbus(const idl::mcp_get_game_news_without_pbus_params& in, idl::mcp_get_game_news_without_pbus_response& out);

	// add by zhengxie for 移动音乐基地 2013.06.27
	int McpServlet::get_mobile_music_info(const idl::mcp_get_mobile_music_info_params& in, idl::mcp_get_mobile_music_info_response& out);
	int McpServlet::set_mobile_music_ptag_by_ring_id(const idl::mcp_set_mobile_music_ptag_by_ring_id_params& in, idl::mcp_set_mobile_music_ptag_by_ring_id_response& out);
	int McpServlet::get_mobile_music_ring_id_by_ptag(const idl::mcp_get_mobile_music_ring_id_by_ptag_params& in, idl::mcp_get_mobile_music_ring_id_by_ptag_response& out);
	// add by zhengxie for pt_game_basic_info 2013.07.04
	int McpServlet::get_pt_game_basic_list_by_game_id(const idl::mcp_get_pt_game_basic_list_by_game_id_params& in, idl::mcp_get_pt_game_basic_list_by_game_id_response& out);
	int McpServlet::get_pt_game_basic_list_by_app_key(const idl::mcp_get_pt_game_basic_list_by_app_key_params& in, idl::mcp_get_pt_game_basic_list_by_app_key_response& out);
	int McpServlet::get_pt_app_key_by_game_id(const idl::mcp_get_pt_app_key_by_game_id_params& in, idl::mcp_get_pt_app_key_by_game_id_response& out);
	int McpServlet::get_pt_game_id_by_app_key(const idl::mcp_get_pt_game_id_by_app_key_params& in, idl::mcp_get_pt_game_id_by_app_key_response& out);
	int McpServlet::exist_pt_game_by_game_id(const idl::mcp_exist_pt_game_by_game_id_params& in, idl::mcp_exist_pt_game_by_game_id_response& out);
	int McpServlet::is_same_pt_game(const idl::mcp_is_same_pt_game_params& in, idl::mcp_is_same_pt_game_response& out);
	int McpServlet::remove_pt_game_basic_list_by_app_key(const idl::mcp_remove_pt_game_basic_list_by_app_key_params& in, idl::mcp_remove_pt_game_basic_list_by_app_key_response& out);
	//add by zhengxie 2013.08.29 争霸赛接口
	int McpServlet::get_championship_info(const idl::mcp_get_championship_info_params& in, idl::mcp_get_championship_info_response& out);
	//add by zhengxie 2013.09.03 用户积分接口
	int McpServlet::user_add_coins(const idl::mcp_user_add_coins_params& in, idl::mcp_user_add_coins_response& out);
	int McpServlet::get_game_news(const idl::mcp_get_game_news_params& in, idl::mcp_get_game_news_response& out);
	int McpServlet::get_ip_province(const idl::mcp_get_ip_province_params& in, idl::mcp_get_ip_province_response& out);
	int McpServlet::get_question_type_name(const idl::mcp_get_question_type_name_params& in, idl::mcp_get_question_type_name_response& out);
	int McpServlet::get_user_question(const idl::mcp_get_user_question_params& in, idl::mcp_get_user_question_response& out);
	int McpServlet::submit_question(const idl::mcp_submit_question_params& in, idl::mcp_submit_question_response& out);
	int McpServlet::get_qp_activity(const idl::mcp_get_qp_activity_params& in, idl::mcp_get_qp_activity_response& out);
	int McpServlet::get_qp_top_activity(const idl::mcp_get_qp_top_activity_params& in, idl::mcp_get_qp_top_activity_response& out);
	int McpServlet::get_qp_activity_person(const idl::mcp_get_qp_activity_person_params& in, idl::mcp_get_qp_activity_person_response& out);
	int McpServlet::get_award_list(const idl::mcp_get_award_list_params& in, idl::mcp_get_award_list_response& out);
	int McpServlet::get_person_award_list(const idl::mcp_get_person_award_list_params& in, idl::mcp_get_person_award_list_response& out);
	int McpServlet::get_person_award(const idl::mcp_get_person_award_params& in, idl::mcp_get_person_award_response& out);
	int McpServlet::get_person_all_awards(const idl::mcp_get_person_all_awards_params& in, idl::mcp_get_person_all_awards_response& out);
	int McpServlet::grab_award(const idl::mcp_grab_award_params& in, idl::mcp_grab_award_response& out);
	int McpServlet::get_qp_rule(const idl::mcp_get_qp_rule_params& in, idl::mcp_get_qp_rule_response& out);
	int McpServlet::get_qp_basic_info(const idl::mcp_get_qp_basic_info_params& in , idl::mcp_get_qp_basic_info_response& out);
	int McpServlet::test_redis(const idl::mcp_test_redis_params& in, idl::mcp_test_redis_response& out);
    int McpServlet::use_exchange_code(const idl::mcp_use_exchange_code_params&, idl::mcp_use_exchange_code_response&);
};


class McpServer : public ubrpc::UbRpcServer
{
	public:
		ubrpc::Servlet * createServlet();
};

#endif
