#ifndef _CONTENT_INFO_H_
#define _CONTENT_INFO_H_

#include <string>
#include <vector>
#include <map>
#include "content_common.h"
#include "mongo_connpool.h"
#include "ub_server.h"

using namespace std;

extern mongo_connpool* mongo_pool;

namespace content
{
    struct download_detail_t
    {
        string id;
        string url;
        string url_type;
        string media_format;
        string size;
        string mtype;
        string charge_id;
        string charge_value;
        string apk_flag; //add by zhengxie
        void clear()
        {
            id.clear();
            url.clear();
            url_type.clear();
            media_format.clear();
            size.clear();
            mtype.clear();
            charge_id.clear();
            charge_value.clear();
        }
    };

    struct cp_info_t
    {
        string id;
        string name;
        void clear()
        {
            id.clear();
            name.clear();
        }
    };

    struct content_detail_t
    {
        string id;
        string name;
        string type;
        vector<class_info_t> class_info;
        vector<img_info_t> img_info;
        string ext_id;
        string abstract;
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

		// [s] add by gwj for mcp_content_pub_prop
        string pub_prop_ids;
		// [e] add by gwj for mcp_content_pub_prop

        cp_info_t cp_info;
        struct grant_detail_t grant_info;
        vector<download_detail_t> download_info;
		void clear()
		{
			id.clear();
			name.clear();
			type.clear();
			ext_id.clear();
			abstract.clear();
			version.clear();
			method.clear();
			star.clear();
			network.clear();
			code.clear();
			log_date.clear();
			mod_date.clear();
			dft_charge_type.clear();
			dft_charge_val.clear();
			enName.clear();
			language.clear();
			adapt.clear();
			package_name.clear();
			version_appinfo.clear();
			sdk_version.clear();
			version_code.clear();
			ptype_0.clear();
			ptype_1.clear();
			ptype_2.clear();
			ptype_3.clear();
			ptype_4.clear();
			cp_info.clear();
			grant_info.clear();
            class_info.clear();
            img_info.clear();
            download_info.clear();

            // [s] add by gwj for mcp_content_pub_prop
            pub_prop_ids.clear();
            // [e] add by gwj for mcp_content_pub_prop
/*
			for(int i=0; i<class_info.size(); i++)
			{
				class_info[i].clear();
			}
			for(int i=0; i<class_info.size(); i++)
			{
				img_info[i].clear();
			}
			for(int i=0; i<class_info.size(); i++)
			{
				download_info[i].clear();
			}
*/
		}
    };

    class ContentInfo
    {
        public:
            ContentInfo(const string& id)
            {
                _id = id;
                _conn = mongo_pool->getConnection();
            }
            ~ContentInfo()
            {
                if(_conn!=NULL){
                    mongo_pool->releaseConnection(_conn);
                }
            }
            void set_adapt_info(const uint32_t adapt_type, const string& adapt_id)
            {
                _adapt_type = adapt_type;
                size_t pos1=0, pos2=0;
                while((pos2=adapt_id.find_first_of(',',pos1))!=string::npos){
                    _adapt_id.push_back(adapt_id.substr(pos1,pos2-pos1));
                    UB_LOG_DEBUG("++++[%s]++++,[%d,%d]",adapt_id.substr(pos1,pos2-pos1).c_str(), (int)pos1, (int)pos2);
                    pos1=pos2+1;
                }
                _adapt_id.push_back(adapt_id.substr(pos1));
                UB_LOG_DEBUG("++++[%s]++++",adapt_id.substr(pos1).c_str());
            }
            int get_content_detail(content_detail_t& detail);
            int get_basic_info(content_detail_t& detail);
            int get_grant_info();
            int get_download_info(vector<download_detail_t>& download_list);
            int getGrabList(int32_t& count, vector<grab_t>& grab_list);

            void get_basic_info_from_bson(bson* bson_out,content_detail_t& detail);
            void get_download_info_from_cursor(mongo_cursor* cursor,vector<download_detail_t>& download_list);

            void get_class_info_from_bson(bson_iterator* it,content_detail_t& detail);
            void get_img_info_from_bson(bson_iterator* it,content_detail_t& detail);

            void free_data( mongo_cursor * cursor, bson* query, bson* fields, bson* bson_result );
        protected:
            string convert_mongo_ts_to_string(bson_iterator* it);
        private:
            mongo *_conn;
            string _id;
            bson _query[1];
            bson _fields[1];
            uint32_t _adapt_type;
            vector<string> _adapt_id;
            content_detail_t _content_detail;

            bson* get_basic_query_bson();
            bson* get_basic_fields_bson();
            bson* get_download_query_bson();
            bson* get_download_fields_bson();
            bson* get_grant_query_bson();
            bson* get_grant_fields_bson();
    };
};

#endif
