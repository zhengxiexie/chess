/*
 @file		mcpsvr_baiduAppInfo.h
 @author	laixiaokang(xiaokanglai@126.com)
 @date		五  1月 10 13:53:54 CST 2014
 @brief		增加百度应用，兼容原有代码，游戏详情(id,packagename)、收藏、下载等
*/
//#include "common.h"
#include "../content_list.h"
#include "mcpsvr.h"
#include "../mcp.idl.mcp.h"
#include "input_type/json_extractor.h"
#include "decorator/single_decorator/is_decorator.h"
#include "decorator/single_decorator/or_decorator.h"
#include "decorator/single_decorator/orderby_decorator.h"
#include "decorator/single_decorator/weight_decorator.h"
#include "decorator/single_decorator/not_equal_decorator.h"
#include "decorator/single_decorator/smaller_decorator.h"
#include "decorator/compound_decorator/decorator_for_test.h"
#include "input_type/content_input.h"
#include "module/mcp_content_data.h"
#include "module/mcp_game_content.h"
#include "output_type/game_update_download_info.h"

#define SCREENSHOTTYPE "8"
#define APPICONTYPE "17"
#define BAIDUAPPIDMINILEN 8

string trim(string& s);

bool parseContentId(int nsize, const string& contentId, vector<string>& baiduIdList, string& strDuokuIdList, string& strBaiduIdList);

//int getBaiduAppInfoFromMongoDb(map<string,string>& paramVec,idl::mcp_get_game_content_info_list_ext_response& out);
int getBaiduAppInfoFromMongoDb(map<string,string>& paramVec,idl::vector<idl::game_content_download_info_t>* out, int nIndex);

//int getBaiduAppInfoFromMongoDbByPackageName(map<string,string>& paramVec,idl::mcp_get_game_dbapp_detail_info_response& out);
int getBaiduAppInfoFromMongoDbByPackageName(map<string,string>& paramVec,idl::mcp_get_game_dbapp_detail_info_response& out, int nIndex);
int get_baiduApp_download_info(map<string,string>& inputMap,idl::mcp_get_game_dbapp_detail_info_response& out);
