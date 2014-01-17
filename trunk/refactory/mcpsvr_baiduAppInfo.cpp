#include "mcpsvr_baiduAppInfo.h"
//#include "common.h"
//#include "../mcpsvr_content_common.h"
//#include "../mcp.idl.mcp.h"

string trim(string& s)
{
    const string drop = " ";    
    s.erase(s.find_last_not_of(drop)+1);   
    return s.erase(0,s.find_first_not_of(drop));
}

bool parseContentId(int nsize, const string& contentId, vector<string>& baiduIdList, string& strDuokuIdList, string& strBaiduIdList)
{
    bool bResult = false;
    int nStart = 0;
    int nPos = 0;
    int nItemLen = 0;
    string strItem;
    baiduIdList.clear();
    strDuokuIdList = "";
    string contentIdTmp = contentId;
    contentIdTmp = trim(contentIdTmp);
    nPos = contentIdTmp.find(';',nStart);
    if(nPos == -1)
    {
        if(contentIdTmp.length() >= nsize)
        {
            baiduIdList.push_back(contentIdTmp);
            strBaiduIdList = contentIdTmp;
        }
        else
        {
            strDuokuIdList = contentIdTmp;
            bResult = true;
        }
        return bResult;
    }
    while(nPos != -1)
    {
        nItemLen = nPos - nStart;
        strItem = contentIdTmp.substr(nStart,nItemLen);
        strItem = trim(strItem);
        nItemLen = strItem.length();
        if(nItemLen >= nsize)
        {
            baiduIdList.push_back(strItem);
            if(strBaiduIdList == "")
            {
                strBaiduIdList=strItem;
            }
            else
            {
                strBaiduIdList = strBaiduIdList + ";" + strItem;
            }
        }
        else
        {
            if(strDuokuIdList == "")
            {
                strDuokuIdList=strItem;
            }
            else
            {
                strDuokuIdList = strDuokuIdList + ";" + strItem;
            }
            bResult = true;
        }
        nStart = nPos + 1;
        nPos = contentIdTmp.find(';',nStart);

        //UB_LOG_FATAL("in %s,strItem=%s",__func__,strItem.data());
    }
    int nLen = contentIdTmp.length();
    nItemLen = nLen - nStart;
    if(nItemLen > 0)
    {
        strItem = contentIdTmp.substr(nStart,nItemLen);
        strItem = trim(strItem);
        nItemLen = strItem.length();
        if(nItemLen >= nsize)
        {
            if(strBaiduIdList == "")
            {
                strBaiduIdList=strItem;
            }
            else
            {
                strBaiduIdList = strBaiduIdList + ";" + strItem;
            }

            baiduIdList.push_back(strItem);
        }
        else
        {
            if(strDuokuIdList == "")
            {
                strDuokuIdList=strItem;
            }
            else
            {
                strDuokuIdList = strDuokuIdList + ";" + strItem;
            }

            bResult = true;
        }
        //UB_LOG_FATAL("in %s,strItem=%s",__func__,strItem.data());
    }
    return bResult;
}

int getBaiduAppInfoFromMongoDb(map<string,string>& paramVec,idl::vector<idl::game_content_download_info_t>* out, int nIndex)
{
    UB_LOG_NOTICE("%s start", __func__);

    int nNumber = 0;
    const char* content_id = (paramVec["content_id"]).data();
    vector<string> idVector;
    string strDuokuIdList;
    string strBaiduIdList;
    parseContentId(BAIDUAPPIDMINILEN,content_id, idVector,strDuokuIdList,strBaiduIdList);
    int nPage = atoi(paramVec["page"].data());
    int nPage_num = atoi(paramVec["page_num"].data());
    int nDuokuAppNum = atoi(paramVec["duokuAppNum"].data());
    string strFunctionFlag = paramVec["fuction_flags"];
    int nStart = (nPage - 1) * nPage_num;
    int nStartIndex = nIndex;

    mongo* conn;
    int j = 0;
	bool isInit = false;
	int nSpace = 0;
    char index[4];
    memset(index, 0x00, sizeof(index));
    if( mcp_connect_mongo_hosts( conn )!= MCP_OK )
    {
        //mongo_destroy( conn );
        UB_LOG_FATAL("Can not connect to MongoDB [%s:%d]", __FILE__, __LINE__ );
        return 0;
    }

    const char* key;
    bson query[1];
    const bson *result;
    string tmp_result;

    string tmp_charge_typename;
    mongo_cursor *cursor = NULL;    

    //设置查询条件
    bson_init(query);
    bson_append_start_object(query,"id");
    bson_append_start_array(query,"$in");
    vector<string>::const_iterator vec_it = idVector.begin();
    //for(; vec_it != idVector.end(); vec_it++)
    if(strFunctionFlag == "yesPage")
    {
        for(; (vec_it != idVector.end()) && (j < nPage_num); vec_it++)
        {
			if(!isInit && nDuokuAppNum >= nStart)
			{
				j = nDuokuAppNum - nStart;
				isInit = true;
			}
			else
			{
				if((nDuokuAppNum + nSpace++) < nStart)
				{
					continue;
				}
			}
            sprintf(index,"%d", j);
            string strId = *vec_it;
            bson_append_int( query, index, atoi(strId.data()));
            //UB_LOG_FATAL("cdata id %d is %s",j,strId.data() );
            j++;
        }
    }
    else
    {
        for(; vec_it != idVector.end(); vec_it++)
        {
            sprintf(index,"%d", j);
            string strId = *vec_it;
            bson_append_int( query, index, atoi(strId.data()));
            //UB_LOG_FATAL("cdata id %d is %s",j,strId.data() );
            j++;
        }

    }
    bson_append_finish_array( query );
    bson_append_finish_object( query );
    bson_finish(query);
    //设置返回域
    //bson_init(fields);
    //bson_append_int(fields,"id",1);
    //bson_append_string(fields,"apkName","apkName");
    //bson_finish(fields);

    cursor = mongo_find(conn, "mcp.mcp_content_baiduAppInfo",query,NULL, 100, 0, 0 );
    if( cursor == NULL )
    {
        mongo_pool->releaseConnection(conn);
        bson_destroy(query);
        //bson_destroy(fields);
        UB_LOG_FATAL( "query content info from [mcp_content_data] faied"  );
        return 0;
    }
    UB_LOG_NOTICE(" Find Data Info From mcp.mcp_content_data OK %s %d",__FILE__,__LINE__);
    while( mongo_cursor_next( cursor ) == MONGO_OK )
    {
        result = mongo_cursor_bson( cursor );
        bson_iterator it[1];
        bson_iterator_init( it, result);

        map<string,string> appInfoColumnMap;
        appInfoColumnMap.clear();

        while( bson_iterator_next(it) ){
            stringstream ss;
            ss.clear();
            key = bson_iterator_key(it);
            if( 0 == strcmp(key,"id")){
                //change_everything_to_string(it,tmp_result);
                ss<<bson_iterator_int(it);
                tmp_result = ss.str();
            }
            //else if( 0 == strcmp(key,"apkName"))
            else
            {
                //change_everything_to_string(it,tmp_result);
                tmp_result = string(bson_iterator_string(it));
            }
            appInfoColumnMap.insert(pair<string,string>(key,tmp_result));
        }
        //idl::game_content_download_info_t* pResultListItem = pResultList->push_back();
        idl::game_content_download_info_t* pResultListItem = out->mutable_get(nIndex++);
        idl::content_basic_info_t* pBasicInfo = pResultListItem->m_basic_info();
        pBasicInfo->set_id(appInfoColumnMap["id"].data());
        pBasicInfo->set_name(appInfoColumnMap["sname"].data());
        pBasicInfo->set_type(appInfoColumnMap["catename"].data());
        pBasicInfo->set_ext_id("0");
        pBasicInfo->set_abstract_(appInfoColumnMap["description"].data());
        pBasicInfo->set_version(appInfoColumnMap["versionname"].data());
        pBasicInfo->set_method("");
        pBasicInfo->set_star(appInfoColumnMap["score"].data());
        pBasicInfo->set_network("");
        pBasicInfo->set_code("");
        pBasicInfo->set_log_date(appInfoColumnMap["releasedate"].data());
        pBasicInfo->set_mod_date(appInfoColumnMap["insertDate"].data());
        pBasicInfo->set_dft_charge_type("");
        pBasicInfo->set_dft_charge_val("");
        pBasicInfo->set_enName(appInfoColumnMap["apkName"].data());
        pBasicInfo->set_language(appInfoColumnMap["language"].data());
        pBasicInfo->set_adapt("");
        pBasicInfo->set_package_name(appInfoColumnMap["apkName"].data());
        pBasicInfo->set_version_appinfo("");
        pBasicInfo->set_sdk_version(appInfoColumnMap["platform_version"].data());
        pBasicInfo->set_version_code(appInfoColumnMap["versioncode"].data());
        pBasicInfo->set_ptype_0("");
        pBasicInfo->set_ptype_1("");
        pBasicInfo->set_ptype_2("");
        pBasicInfo->set_ptype_3("");
        pBasicInfo->set_ptype_4(appInfoColumnMap["all_download"].data());
        //idl::vector<idl::string>* pPub_propsVec = pBasicInfo->m_pub_props();
        idl::vector<idl::pub_prop_info_t>* pPropInfo = pBasicInfo->m_pub_prop_info();
        idl::pub_prop_info_t* pPropInfoItem = pPropInfo->push_back();
        pPropInfoItem->set_name("");
        pPropInfoItem->set_color("");
        pBasicInfo->set_union_start_action("");
        pBasicInfo->set_has_account("");
        pBasicInfo->set_editor_recom("");
        pBasicInfo->set_show_highspeed("");
        pBasicInfo->set_bind_resource_id("0");
        idl::vector<idl::class_info_t>* pClassInfo = pBasicInfo->m_class_info();
        idl::class_info_t* pClassInfoItem = pClassInfo->push_back();
        pClassInfoItem->set_id("0");
        pClassInfoItem->set_name(appInfoColumnMap["catename"].data());
        idl::vector<idl::img_info_t>* pImgInfo = pBasicInfo->m_img_info();
        idl::img_info_t* pImgInfoIconItem = pImgInfo->push_back();
        pImgInfoIconItem->set_type(APPICONTYPE);
        pImgInfoIconItem->set_url(appInfoColumnMap["icon"].data());
        string strScreenShot = appInfoColumnMap["screenshot"].data();
        vector<string> screenShotVec;
        string strTmp1,strTmp2;
        parseContentId(BAIDUAPPIDMINILEN,strScreenShot,screenShotVec,strTmp1,strTmp2);
        for(unsigned int i = 0; i < screenShotVec.size(); i++)
        {
            idl::img_info_t* pImgInfoItem = pImgInfo->push_back();
            pImgInfoItem->set_type(SCREENSHOTTYPE);
            pImgInfoItem->set_url(screenShotVec.at(i).data());
        }
        idl::cp_info_t* pCpInfo = pBasicInfo->m_cp_info();
        pCpInfo->set_id("0");
        pCpInfo->set_name("");
        idl::grant_detail_t* pGrantDetail = pBasicInfo->m_grant_info();
        pGrantDetail->set_id("0");
        pGrantDetail->set_grant_time("");
        pGrantDetail->set_valid_time("");
        pGrantDetail->set_modify_time("");

        idl::vector<idl::download_detail_t>* pDownLoadInfo = pResultListItem->m_download_info();
        idl::download_detail_t* pDownLoadInfoItem = pDownLoadInfo->push_back();
        pDownLoadInfoItem->set_id("0");
        pDownLoadInfoItem->set_url(appInfoColumnMap["url"].data());
        pDownLoadInfoItem->set_baidu_url(appInfoColumnMap["url"].data());
        pDownLoadInfoItem->set_url_type("");
        pDownLoadInfoItem->set_media_format(appInfoColumnMap["packageformat"].data());
        pDownLoadInfoItem->set_size(appInfoColumnMap["packagesize"].data());
        pDownLoadInfoItem->set_mtype("");
        pDownLoadInfoItem->set_charge_id("0");
        pDownLoadInfoItem->set_charge_value(appInfoColumnMap["fee"].data());
        pDownLoadInfoItem->set_apk_flag(appInfoColumnMap["packageformat"].data());
        pDownLoadInfoItem->set_channel_info("GCenter");
        pDownLoadInfoItem->set_channel_id("0");
        pDownLoadInfoItem->set_package_name(appInfoColumnMap["apkName"].data());
        pDownLoadInfoItem->set_int_version(appInfoColumnMap["versioncode"].data());
        pDownLoadInfoItem->set_string_version(appInfoColumnMap["versionname"].data());
        pDownLoadInfoItem->set_sdk_version(appInfoColumnMap["platform_version"].data());
        pDownLoadInfoItem->set_apk_md5("");

        nNumber++;
    }

    bson_destroy(query);
    mongo_pool->releaseConnection(conn);
    
    return nNumber;
}

int getBaiduAppInfoFromMongoDbByPackageName(map<string,string>& paramVec,idl::mcp_get_game_dbapp_detail_info_response& out, int nIndex)
{
    UB_LOG_NOTICE("%s start", __func__);
    const char* package_name = (paramVec["package_name"]).data();
    string strTmp1,strTmp2;
    vector<string> packageNameList;
    packageNameList.clear();
    parseContentId(0,package_name,packageNameList,strTmp1,strTmp2);

    //idl::vector<idl::game_update_download_info>* pResultList = out.m_result_params()->m_result_list();

    mongo* conn;
    vector<string>::iterator vec_it;
    int j = 0;
    char index[4];
    memset(index, 0x00, sizeof(index));
    if( mcp_connect_mongo_hosts( conn )!= MCP_OK )
    {
        //mongo_destroy( conn );
        UB_LOG_FATAL("Can not connect to MongoDB [%s:%d]", __FILE__, __LINE__ );
        return MCP_NG;
    }

    const char* key;
    bson query[1];
    //bson fields[1];
    const bson *result;
    string tmp_result;

    string tmp_charge_typename;
    mongo_cursor *cursor = NULL;    

    //设置查询条件
    bson_init(query);
    bson_append_start_object(query,"apkName");
    bson_append_start_array(query,"$in");
    vector<string>::const_iterator iter = packageNameList.begin();
    for(; iter != packageNameList.end(); iter++)
    {
        sprintf(index,"%d", j);
        string packageName = *iter;
        bson_append_string( query, index, packageName.data());
        UB_LOG_NOTICE("cdata id %d is %s",j,packageName.data());
        j++;
    }
    bson_append_finish_array( query );
    bson_append_finish_object( query );
    bson_finish(query);

    cursor = mongo_find(conn, "mcp.mcp_content_baiduAppInfo",query,NULL, 100, 0, 0 );
    if( cursor == NULL )
    {
        mongo_pool->releaseConnection(conn);
        bson_destroy(query);
        //bson_destroy(fields);
        UB_LOG_FATAL( "query content info from [mcp_content_data] faied"  );
        return MCP_NG;
    }
    UB_LOG_NOTICE(" Find Data Info From mcp.mcp_content_data OK %s %d",__FILE__,__LINE__);
    int nCounter; //res
    nCounter = 0;
    while( mongo_cursor_next( cursor ) == MONGO_OK )
    {
        ++nCounter;
        result = mongo_cursor_bson( cursor );
        bson_iterator it[1];
        bson_iterator_init( it, result);

        map<string,string> appInfoColumnMap;
        appInfoColumnMap.clear();

        while( bson_iterator_next(it) ){
            stringstream ss;
            ss.clear();
            key = bson_iterator_key(it);
            if( 0 == strcmp(key,"id")){
                //change_everything_to_string(it,tmp_result);
                ss<<bson_iterator_int(it);
                tmp_result = ss.str();
            }
            //else if( 0 == strcmp(key,"apkName"))
            else
            {
                //change_everything_to_string(it,tmp_result);
                tmp_result = string(bson_iterator_string(it));
            }
            appInfoColumnMap.insert(pair<string,string>(key,tmp_result));
        }
        //m_result_params
        idl::game_update_download_info* pDownloadInfoItem = out.m_result_params()->m_result_list()->mutable_get(nIndex++);
        idl::content_basic_info_t* pBasicInfo = pDownloadInfoItem->m_basic_info();
        pBasicInfo->set_id(appInfoColumnMap["id"].data());
        pBasicInfo->set_name(appInfoColumnMap["sname"].data());
        pBasicInfo->set_type(appInfoColumnMap["catename"].data());
        pBasicInfo->set_ext_id("0");
        pBasicInfo->set_abstract_(appInfoColumnMap["description"].data());
        pBasicInfo->set_version(appInfoColumnMap["versionname"].data());
        pBasicInfo->set_method("");
        pBasicInfo->set_star(appInfoColumnMap["score"].data());
        pBasicInfo->set_network("");
        pBasicInfo->set_code("");
        pBasicInfo->set_log_date(appInfoColumnMap["releasedate"].data());
        pBasicInfo->set_mod_date(appInfoColumnMap["insertDate"].data());
        pBasicInfo->set_dft_charge_type("dft_charge_type");
        pBasicInfo->set_dft_charge_val("dft_charge_val");
        pBasicInfo->set_enName(appInfoColumnMap["apkName"].data());
        pBasicInfo->set_language(appInfoColumnMap["language"].data());
        pBasicInfo->set_adapt("");
        pBasicInfo->set_package_name(appInfoColumnMap["apkName"].data());
        pBasicInfo->set_version_appinfo("");
        pBasicInfo->set_sdk_version(appInfoColumnMap["platform_version"].data());
        pBasicInfo->set_version_code(appInfoColumnMap["versioncode"].data());
        pBasicInfo->set_ptype_0("");
        pBasicInfo->set_ptype_1("");
        pBasicInfo->set_ptype_2("");
        pBasicInfo->set_ptype_3("");
        pBasicInfo->set_ptype_4(appInfoColumnMap["all_download"].data());
        //idl::vector<idl::string>* pPub_propsVec = pBasicInfo->m_pub_props();
        idl::vector<idl::pub_prop_info_t>* pPropInfo = pBasicInfo->m_pub_prop_info();
        idl::pub_prop_info_t* pPropInfoItem = pPropInfo->push_back();
        pPropInfoItem->set_name("");
        pPropInfoItem->set_color("");
        pBasicInfo->set_union_start_action("");
        pBasicInfo->set_has_account("");
        pBasicInfo->set_editor_recom("");
        pBasicInfo->set_show_highspeed("");
        pBasicInfo->set_bind_resource_id("0");
        idl::vector<idl::class_info_t>* pClassInfo = pBasicInfo->m_class_info();
        idl::class_info_t* pClassInfoItem = pClassInfo->push_back();
        pClassInfoItem->set_id("0");
        pClassInfoItem->set_name(appInfoColumnMap["catename"].data());
        idl::vector<idl::img_info_t>* pImgInfo = pBasicInfo->m_img_info();
        string strScreenShot = appInfoColumnMap["screenshot"].data();
        vector<string> screenShotVec;
        string strTmp1,strTmp2;
        parseContentId(BAIDUAPPIDMINILEN,strScreenShot,screenShotVec,strTmp1,strTmp2);
        for(unsigned int i = 0; i < screenShotVec.size(); i++)
        {
            idl::img_info_t* pImgInfoItem = pImgInfo->push_back();
            pImgInfoItem->set_type(SCREENSHOTTYPE);
            pImgInfoItem->set_url(screenShotVec.at(i).data());
        }
        idl::cp_info_t* pCpInfo = pBasicInfo->m_cp_info();
        pCpInfo->set_id("0");
        pCpInfo->set_name("");
        idl::grant_detail_t* pGrantDetail = pBasicInfo->m_grant_info();
        pGrantDetail->set_id("0");
        pGrantDetail->set_grant_time("");
        pGrantDetail->set_valid_time("");
        pGrantDetail->set_modify_time("");

        idl::vector<idl::download_detail_t>* pDownLoadInfo = pDownloadInfoItem->m_download_info();
        idl::download_detail_t* pDownLoadInfoItem = pDownLoadInfo->push_back();
        pDownLoadInfoItem->set_id("0");
        pDownLoadInfoItem->set_url(appInfoColumnMap["url"].data());
        pDownLoadInfoItem->set_baidu_url(appInfoColumnMap["url"].data());
        pDownLoadInfoItem->set_url_type("");
        pDownLoadInfoItem->set_media_format(appInfoColumnMap["packageformat"].data());
        pDownLoadInfoItem->set_size(appInfoColumnMap["packagesize"].data());
        pDownLoadInfoItem->set_mtype("");
        pDownLoadInfoItem->set_charge_id("0");
        pDownLoadInfoItem->set_charge_value(appInfoColumnMap["fee"].data());
        pDownLoadInfoItem->set_apk_flag(appInfoColumnMap["packageformat"].data());
        pDownLoadInfoItem->set_channel_info("GCenter");
        pDownLoadInfoItem->set_channel_id("0");
        pDownLoadInfoItem->set_package_name(appInfoColumnMap["apkName"].data());
        pDownLoadInfoItem->set_int_version(appInfoColumnMap["versioncode"].data());
        pDownLoadInfoItem->set_string_version(appInfoColumnMap["versionname"].data());
        pDownLoadInfoItem->set_sdk_version(appInfoColumnMap["platform_version"].data());
        pDownLoadInfoItem->set_apk_md5("");
    }

    out.m_result_params()->set_count(nIndex);
    bson_destroy(query);
    mongo_pool->releaseConnection(conn);
    
    return 0;
}

int get_baiduApp_download_info(map<string,string>& inputMap,idl::mcp_get_game_dbapp_detail_info_response& out)
{
	struct timeval start, middle, end;
	gettimeofday(&start, NULL);

	JsonExtractor extractor;
	extractor.parse_input_params(inputMap["filter"].data());

	const char* channel_info = extractor.get_filter_condition("channel_info");
	const char* channel_id = extractor.get_filter_condition("channel_id");
	const char* not_class_id = extractor.get_filter_condition("no_class_id");

	const char* package_name = inputMap["package_name"].data();
	const char* game_img = inputMap["game_img"].data(); // 用于基本信息输出时的图片类型

	LOGA("[ZX] %s: package_name[%s],game_img[%s], channel_info[%s], channel_id[%s], not_class_id[%s]",__FUNCTION__, package_name, game_img, channel_info, channel_id, not_class_id);

	content_input_params game_imgs; // 统一使用的图片类型
									// 注意：game_img并不是过滤条件，而是筛选条件，即使相应img_type不存在的， 
									// 记录也会输出，只不过没有game_img相关信息罢了 
									
	SET_INPUT_PARAMETERS_TEMPLATE(game_img, game_imgs.img_types);

	AbstractCompoundDecorator content_diff_decor;
	AbstractCompoundDecorator game_basic_decor;
	AbstractCompoundDecorator content_data_decor; 

	OR_CONDITION_JSON(channel_info, "channel_info", content_data_decor);
	OR_CONDITION_JSON(channel_id, "channel_id", content_data_decor);

	int max_size = 0;

	vector<char*> pkg_list;
	int package_name_size = CommonInterface::splic_string((char*)package_name, pkg_list, ",");
	max_size = package_name_size;

    /*
	vector<char*> version_code_list;
	int version_code_size = CommonInterface::splic_string((char*)dev_version_code, version_code_list, ",");
	max_size = (version_code_size > max_size) ? version_code_size:max_size;

	vector<char*> version_list;
	int version_size = CommonInterface::splic_string((char*)dev_version, version_list, ",");
	max_size = (version_size > max_size) ? version_size:max_size;

	vector<char*> apk_md5_list;
	int apk_md5_size = CommonInterface::splic_string((char*)apk_md5, apk_md5_list, ",");
	max_size = (apk_md5_size > max_size) ? apk_md5_size:max_size;

	
	vector<char*> file_md5_list; // 如果要获得增量更新信息，则必填file_md5
	int file_md5_size = CommonInterface::splic_string((char*)file_md5, file_md5_list, ",");
	max_size = (file_md5_size > max_size) ? file_md5_size:max_size;
    */

	int out_record_index = 0; // 最终的count是根据基本信息的条数决定的
							  // 按照基本信息的条数逐条输出总的结果集
	for(int i = 0; i < max_size; i++)
	{

		char* pkg_name = NULL;
		char* ver_code = NULL;
		char* ver_str  = NULL;

		if(i < package_name_size) {
			pkg_name = pkg_list[i];
		}

        /*
		if(i < version_code_size) {
			ver_code = version_code_list[i];
		}

		if(i < version_size) {
			ver_str = version_list[i];
		}

		if(i < apk_md5_size) {
			apk_md5 = apk_md5_list[i];
		}

		if(i < file_md5_size) {
			file_md5 = file_md5_list[i];
		}
        */


		/* 查询下载信息 beg */ 

		//IS_CONDITION_TEMPLATE(file_md5, "file_md5", content_data_decor)
		IS_CONDITION_TEMPLATE(pkg_name, "package_name", content_data_decor)
		IS_CONDITION_TEMPLATE(ver_code, "int_version", content_data_decor)
		IS_CONDITION_TEMPLATE(ver_str, "string_version", content_data_decor)
		//IS_CONDITION_TEMPLATE(apk_md5, "apk_md5", content_data_decor)

		MCPContentData mcp_content_data("c_id");
		mcp_content_data.load_decorator(&content_data_decor);
		mcp_content_data.general_fetch_list_records(false);

		REMOVE_IS_CONDITION_TEMPLATE(pkg_name, content_data_decor)
		REMOVE_IS_CONDITION_TEMPLATE(ver_code, content_data_decor)
		REMOVE_IS_CONDITION_TEMPLATE(ver_str, content_data_decor)

		int data_rec_count = mcp_content_data.get_key_size();
		if(data_rec_count == 0)
		{
			continue;
		}

		/* 查询下载信息 end */ 
		
		/* 查询基本信息 beg */ 

		OrDecorator id_or_dec;			// 如果有下载信息，则需要用于基本信息的c_id修饰器
		id_or_dec.set_same_field_filter_conditions(mcp_content_data.get_result_keys(),"id",true);
		game_basic_decor.add_primary_table_decorator(&id_or_dec);

		MCPGameContent content_game("id");
		NOT_EQUAL_CONDITION_JSON(not_class_id, "class.id", game_basic_decor) // 统一过滤掉的 class.id
				
		content_game.load_decorator(&game_basic_decor);
		content_game.general_fetch_list_records(false);

		game_basic_decor.remove_all_primary_table_decorator();
		int basic_rec_count = content_game.get_key_size();
		if(basic_rec_count == 0)
		{
			//LOGA("[ZX] %s: Game_Basic_Info Not Found!", __FUNCTION__);
			continue;		// 如果基本信息都没有，那就不用输出任何信息
		}

		/* 查询基本信息 end */ 

        MCPDiffUpdateInfo mcp_diff_update_module("old_file_md5");
        /* 插入结果集 beg*/
        game_update_info_param::set_outparameter(out.m_result_params()->mutable_result_list(out_record_index),
												 game_imgs, mcp_content_data, content_game, mcp_diff_update_module, 0);
		out_record_index++; //最终的count是根据基本信息的条数决定的

		/* 插入结果集 end*/


		gettimeofday(&middle, NULL);
		double process_time = CommonInterface::get_interface_process_time(start, middle);
		if(process_time > 200)
		{
			LOGA("[ZX] %s: brk, too long!", __FUNCTION__);
			break;
		}
	}
	
	out.mutable_result_params()->set_count(out_record_index);
    //out.mutable_result_params()->set_count(0);
    LOGA("[ZX] %s: count [%d] OK!", __FUNCTION__, out_record_index);
    gettimeofday(&end, NULL);
    LOGT(start, end);
    return out_record_index;
}
