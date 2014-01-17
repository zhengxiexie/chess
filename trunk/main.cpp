#include "ub.h"
#include "cfgflag.h"
#include "mcpsvr.h"
#include "UbMonitor.h"
#include "ub_server_monitor.h"
#include <bsl/var/String.h>
#include <bsl/string.h>
#include "mcp_session.h"
#include "UADicTree.h"
#include "mongo.h"
#include "mongo_connpool.h"
#include "redis_connpool.h"
#include "content_list.h"
#include "ip_gateway.h"
#include <pthread.h>
#include "./refactory/redis_module/redis_module.h"
#include "./refactory/client/community_client.h"

using namespace std;

CommunityClient community_client;

string IP;
list<string> g_ip_scope;
char REDIS_HOST[16];
char REDIS_PORT[6];
char MCP_MONGODB_HOST[2][16];
char MCP_MONGODB_PORT[2][6];
int mongo_port[2];
std::string mongo_host[2];

uint32_t g_session_tmout;
uint32_t g_redis_db;

int mcpupdate_flag=0;
const int phonevalid_update_flag =1;
map<int,int> invalid_flag;
//mongo conn[1];

bsl::var::Ref g_monitor_info;
char g_proc_name[1024];
MyclientPool * database_pool;
mongo_connpool * mongo_pool = NULL;
redisConnPool *redis_pool;
bsl::string redis_server_addr;
int redis_server_port;
int redis_server_max_size;
UADicTree uaTree;
UAJsonDicTree uaJsonTree;

//add by zhengxie
string get_ip()
{
	string ip;
	char               buf[100];  
	int                ret = 0;  
	struct addrinfo    hints;  
	struct addrinfo    *res, *curr;  
	struct sockaddr_in *sa;  

	bzero(&hints, sizeof (hints));  
	hints.ai_flags = AI_CANONNAME;  
	hints.ai_family = AF_INET;  
	if (gethostname(buf, sizeof (buf)) < 0) {  
		perror("gethostname");  
		return NULL;   
	}     
	if((ret = getaddrinfo(buf, NULL, &hints, &res)) != 0) {  
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(ret));  
		return NULL;   
	}     
	curr = res;  
	while (curr && curr->ai_canonname) 
	{  
		sa = (struct sockaddr_in *)curr->ai_addr;  
		ip =string( inet_ntop(AF_INET, &sa->sin_addr.s_addr, buf, sizeof (buf) ) );
		//printf("name: %s\nip:%s\n\n", curr->ai_canonname,   
				//inet_ntop(AF_INET, &sa->sin_addr.s_addr, buf, sizeof (buf)));  
		curr = curr->ai_next;  	
	}
	return ip;
}

int get_ip_scope()
{
	string key = "ip_list";

	RedisModule redis_processor;
	int result = redis_processor.get_value_list(key.c_str(), g_ip_scope);
	return result;
}

bsl::var::IVar &get_service_monitor_list(bsl::ResourcePool &rp)
{
    (void )rp;
    return g_monitor_info;
}
void get_proc_name() 
{
    pid_t pid = getpid();
    char exec_file[1024];
    char proc_name[1024]; 
    snprintf(exec_file, sizeof(exec_file), "/proc/%d/exe", pid); 
    int ret = readlink(exec_file, proc_name, sizeof(proc_name)); 
    proc_name[ret] = 0; 
    //??ȡ?ļ??? 
    char *p = strrchr(proc_name, '/'); 
    snprintf(g_proc_name, sizeof(g_proc_name), "%s", p + 1); 
}
/**
 *@query server
**/
static int query_server_callback()
{
	nshead_t *req_head = (nshead_t *) ub_server_get_read_buf();
	//nshead_t *res_head = (nshead_t *) ub_server_get_write_buf();
	//void *req_buf = req_head+1;
	//void *res_buf = res_head+1;
	
	ub_log_setbasic(UB_LOG_REQSVR, "%s", req_head->provider);
	ub_log_setbasic(UB_LOG_SVRNAME, "%s", "query");
	//ub_log_setbasic(UB_LOG_CMDNO, "%d", cmd_no);
	
	//res_head->body_len = 0;
	return 0;
}

/**
 *@update server
**/
static int update_server_callback()
{
	nshead_t *req_head = (nshead_t *) ub_server_get_read_buf();
	//nshead_t *res_head = (nshead_t *) ub_server_get_write_buf();
	//void *req_buf = req_head+1;
	//void *res_buf = res_head+1;
	
	ub_log_setbasic(UB_LOG_REQSVR, "%s", req_head->provider);
	ub_log_setbasic(UB_LOG_SVRNAME, "%s", "update");
	//ub_log_setbasic(UB_LOG_CMDNO, "%d", cmd_no);
	
	//res_head->body_len = 0;
	return 0;
}

/**
 *@brief timer
**/

map<int ,struct Data *> database_map;
		
int CreateMap()
{
				UB_LOG_NOTICE("queryPhoneArea is start");
				MyclientRes data_res;
        char query_sql[1024];
        int ret,err,count_num;
        memset(query_sql, 0x00, sizeof(query_sql));
        UB_LOG_NOTICE("fetchConnection is start");
        MysqlConnection * database_connection = database_pool->fetchConnection("default_mysql", &err, 0);
        if ( NULL == database_connection )
        {
            UB_LOG_FATAL("get database connection failed from database_pool");
            data_res.free();
            return 0;
        }
       sprintf(query_sql,
                    "SELECT phone_area_info.* FROM phone_area_info "
			);
			 UB_LOG_NOTICE(" query database is start");
        UB_LOG_NOTICE("%s",query_sql);
			 count_num = database_connection->query(query_sql, &data_res, true);
			   if ( count_num < 0 )
        {
            UB_LOG_FATAL("query data from database failed");
            ret = database_pool->putBackConnection(database_connection, false);
            if (ret != 0)
            {
               ul_writelog(UL_LOG_FATAL, "putback connection is failed");
            }
            data_res.free();
            return 0;
        }
        else{ 
		UB_LOG_NOTICE("get database is ok");
		UB_LOG_NOTICE("data1 is %s",data_res[0][0]);
		UB_LOG_NOTICE("data1 is %s",data_res[2][0]);
		UB_LOG_NOTICE("data1 is %s",data_res[4][0]);
		UB_LOG_NOTICE("data1 is %s",data_res[7][0]);
		}			
	for(int i = 0;i<count_num;i++)
	{
		Data * da = new Data;
		string temp_data;
	   	da->numseg = data_res[i][0];
       		da->type = data_res[i][1];
       		da->oid =  data_res[i][2];
       		da->province =  data_res[i][3];
       		da->city =  data_res[i][4];
       		da->code =  data_res[i][5];
       		da->enabled =  data_res[i][6];
		temp_data = da->numseg.substr(1,6);
       		database_map.insert(pair<int,Data *>(atoi(temp_data.c_str()),da));
			}
			data_res.free();
      ret = database_pool->putBackConnection(database_connection, false);
      if (ret != 0)
        {
           ul_writelog(UL_LOG_FATAL, "putback connection is failed");
           return 0;
        }
        UB_LOG_NOTICE("queryPhoneArea is end");
			return 1;
			
}

//2013.09.18 add uajson thread fuction by lxk
void* uaJson_create_thread(void *)
{
    uaJsonTree.create_uajson_list();
}

void* ip_scope_create_thread(void *)
{
	get_ip_scope();
}

static int monitor_timer(void *)
{
//	UB_LOG_NOTICE("call timer");
	return 0;
}
/**
 * @brief ע??main??????֧???????в???????
 * ?????? ./xxxx -h ??ʾ????
**/
int main(int argc, char **argv)
{
	IP = get_ip();
	ub_t * fw = ub_init("chess", argc, argv);	//??ʼ??????
	if (fw == NULL) { 	//????0??ʾҪ?˳?
		return 0; 
	}
	//comcfg::CmdOption opt;//????һ???????????е???
	//opt.init(argc, argv, "");//??֪??opt??ôʹ?õĲο?public/configure??ʹ??
	//opt["?????е?key??"].to_cstr();//?????ַ??????֪???Ƿ????ڣ??ȵ???opt.hasOption(char *)?ж?һ??

	ub_conf_data_t *conf = ub_open_conf(fw);	//?????????ļ?
	UBFW_ASSERT(conf != NULL, "open configure %s/%s error", fw->path, fw->file);

	ubfw_open_galileo_log(fw);  //????Galileo??־
	//????comlog
	UBFW_ASSERT(ub_load_log(fw) == 0, "open log error");
	ub_svr_vec_t *svrvec = ub_svr_vec_create();	//???᷵??NULL??????û?ڴ??ˣ??????Զ??˳?
	UBFW_ASSERT(svrvec != NULL, "create ub_server_vec_error");
	//????????
	
    ub::UbMonitor *monitor = ub_load_monitor(fw);
    UBFW_ASSERT(fw->conf_build || monitor != NULL, "create UbMonitor error ");

    bsl::ResourcePool rp;
    g_monitor_info = rp.create<bsl::var::Dict>();
    get_proc_name();
    g_monitor_info[g_proc_name] = rp.create<bsl::var::Dict>();
    g_monitor_info[g_proc_name]["__global"] = rp.create<bsl::var::Dict>();
    g_monitor_info[g_proc_name]["__global"]["RUNNING_TIME"] = rp.create<bsl::var::String>("ģ??????ʱ??"); 
    g_monitor_info[g_proc_name]["__global"]["PROGRAM_VERSION"] = rp.create<bsl::var::String>("?????汾??");
    g_monitor_info[g_proc_name]["__global"]["LIB_VERSION"] = rp.create<bsl::var::String>("t?ӿ??汾"); 
    g_monitor_info[g_proc_name]["__global"]["DATA_VERSION"] = rp.create<bsl::var::String>("???ݰ汾"); 
    int max_server_num = 0;    	//????ubserver	query
	ub_server_t *querysvr = ub_load_svr(fw, "query");	//???? query server
	UBFW_ASSERT(fw->conf_build || querysvr != NULL, "create query server error");
	ub_server_setoptsock(querysvr, UBSVR_NODELAY);
	ub_server_set_callback(querysvr, query_server_callback);	//???÷??????ص?
	ub_svr_vec_add(svrvec, querysvr, fw);	//??server???ӵ???????ͳһ????
//    ub_server_set_monitor(querysvr, monitor);
//    g_monitor_info[g_proc_name][ub_server_get_server_name(querysvr)] = get_monitor_list(rp);
    if (max_server_num < ub_server_get_threadnum(querysvr)) {
        max_server_num = ub_server_get_threadnum(querysvr);
    }
	
	//????ubserver	update
	ub_server_t *updatesvr = ub_load_svr(fw, "update");	//???? update server
	UBFW_ASSERT(fw->conf_build || updatesvr != NULL, "create update server error");
	ub_server_setoptsock(updatesvr, UBSVR_NODELAY);
	ub_server_set_callback(updatesvr, update_server_callback);	//???÷??????ص?
	ub_svr_vec_add(svrvec, updatesvr, fw);	//??server???ӵ???????ͳһ????
//    ub_server_set_monitor(updatesvr, monitor);
//    g_monitor_info[g_proc_name][ub_server_get_server_name(updatesvr)] = get_monitor_list(rp);
    if (max_server_num < ub_server_get_threadnum(updatesvr)) {
        max_server_num = ub_server_get_threadnum(updatesvr);
    }
	
    monitor->set_query_item_callback(get_service_monitor_list);	//????ubclient
	//????????д??Ҫ???ص?????

	//???ü???????, ???????Զ?????????ģʽ??????????Ӧ???˳???????
	ub_builddone_exit(fw);

        int ret= 0;
        database_pool = new MyclientPool();

        if (database_pool == NULL)
        {
            UB_LOG_FATAL("database pool create is failed");
            return -1;
        }

        ret = database_pool->init(false);
        if (ret != 0)
        {
           UB_LOG_FATAL("database pool init is failed");
           return -1;
        }

        ret = database_pool->addServer("./conf", "queryf.conf");
        if (ret != 0)
        {
            UB_LOG_FATAL("database poll add server from config is failed");
            return -1;
        }
        UB_LOG_NOTICE("create database pool handle ok!");
        ret = ub_conf_getnstr( conf, "redis_host", REDIS_HOST, sizeof(REDIS_HOST), NULL,NULL);
        if ( 0 != ret )
        {
            UB_LOG_FATAL("get REDIS_HOST config failed, ret is %d", ret);
            return -1;
        }

        ret = ub_conf_getnstr( conf, "redis_port", REDIS_PORT, sizeof(REDIS_PORT), NULL,NULL);
        if ( 0 != ret )
        {
            UB_LOG_FATAL("get REDIS_PORT config failed, ret is %d", ret);
            return -1;
        }
        char conf_buf[32];
        memset( conf_buf, 0x00, sizeof( conf_buf ) );
        ret = ub_conf_getnstr( conf, "session_time_out", conf_buf, sizeof(conf_buf), NULL,NULL);
        if ( 0 != ret )
        {
            UB_LOG_FATAL("get session_time_out config failed, ret is %d", ret);
            g_session_tmout = 600;
        }
        else
        {
            g_session_tmout = atoi( conf_buf );
            if( g_session_tmout <= 0 )
            {
                g_session_tmout = 600;
            }
        }
        int rett = CreateMap();
		if(rett)
		{
			UB_LOG_NOTICE("get PhoneArea is ok");
		}
		else 
		{
			UB_LOG_NOTICE("get PhoneArea is false");
		}  
		memset( conf_buf, 0x00, sizeof( conf_buf ) );
		ret = ub_conf_getnstr( conf, "redis_db", conf_buf, sizeof(conf_buf), NULL,NULL);
		if ( 0 != ret )
		{
			UB_LOG_FATAL("get redis_db config failed, ret is %d", ret);
			g_redis_db = 0;
		}
		else
			g_redis_db = atoi( conf_buf );

		ret = ub_conf_getnstr( conf, "mongodb_host0", MCP_MONGODB_HOST[0], sizeof(MCP_MONGODB_HOST[0]), NULL,NULL);
		if ( 0 != ret )
		{
			UB_LOG_FATAL("get MCP_MONGODB_HOST0 config failed, ret is %d", ret);
			return -1;
		}

		ret = ub_conf_getnstr( conf, "mongodb_port0", MCP_MONGODB_PORT[0], sizeof(MCP_MONGODB_HOST[0]), NULL,NULL);
		if ( 0 != ret )
		{
			UB_LOG_FATAL("get MCP_MONGODB_PORT0 config failed, ret is %d", ret);
			return -1;
		}

		ret = ub_conf_getnstr( conf, "mongodb_host1", MCP_MONGODB_HOST[1], sizeof(MCP_MONGODB_HOST[1]), NULL,NULL);
		if ( 0 != ret )
		{
			UB_LOG_FATAL("get MCP_MONGODB_HOST1 config failed, ret is %d", ret);
			return -1;
		}


		ret = ub_conf_getnstr( conf, "mongodb_port1", MCP_MONGODB_PORT[1], sizeof(MCP_MONGODB_HOST[1]), NULL,NULL);
		if ( 0 != ret )
		{
			UB_LOG_FATAL("get MCP_MONGODB_PORT1 config failed, ret is %d", ret);
			return -1;
		}

		mongo_port[0] = mongo_port[1] = atoi( MCP_MONGODB_PORT[0] );
		mongo_host[0] = MCP_MONGODB_HOST[0];
		mongo_host[1] = MCP_MONGODB_HOST[1];


		//mongo_connpool pool;
		mongo_pool = mongo_connpool::getInstance();
		if( mongo_pool == NULL )
		{
			UB_LOG_FATAL( "MongoDB getInstance failed, [%s:%d]", __FILE__, __LINE__ );
			return -1;
		}
	


		char redis_addr[256];
        memset(redis_addr,0x00,sizeof(redis_addr));
        ret = ub_conf_getnstr( conf, "redis_host", redis_addr, 256, NULL,NULL);
        if ( 0 != ret )
        {   
            UB_LOG_FATAL("get redis server addr config failed, ret is %d", ret);
            return -1; 
        }   
        redis_server_addr.appendf(redis_addr);
    
        char redis_port[6];
        memset(redis_port,0x00,sizeof(redis_port));
        ret = ub_conf_getnstr( conf, "redis_port", redis_port,6, NULL,NULL);
        if ( 0 != ret )
        {   
            UB_LOG_FATAL("get redis server addr config failed, ret is %d", ret);
            return -1; 
        }   
        redis_server_port = atoi(redis_port);
    
        char redis_max_size[6];
        memset(redis_max_size,0x00,sizeof(redis_max_size));
        ret = ub_conf_getnstr( conf, "redis_server_max_size", redis_max_size,6, NULL,NULL);
        if ( 0 != ret )
        {   
            UB_LOG_FATAL("get redis server addr config failed, ret is %d", ret);
            return -1; 
        }   
        redis_server_max_size = atoi(redis_max_size);
    
        redis_pool = redisConnPool::getInstance();

		IpGateway::getInstance();
		uaTree.create_trie_list();

        pthread_t uajson_thread;
        pthread_create(&uajson_thread, NULL, uaJson_create_thread, NULL); //2013.09.18

        pthread_t ip_scope_thread;
        pthread_create(&ip_scope_thread, NULL, ip_scope_create_thread, NULL); //2013.12.16

		//content::content_list::get_content_class_id_name_map();
		//content::content_list::get_content_charge_id_name_map();
		content::mcp_content_map::init();

		community_client.init();

		McpServer svr;
		svr.registerWithUb(querysvr);
		svr.registerWithUb(updatesvr);

		UBFW_ASSERT(ub_svr_vec_run(svrvec) == 0, "run server vectors error");	//????server
		ub_timer_task_t *timer = ub_create_timer_task();
		UBFW_ASSERT(timer!=0, "create timer task error");
		UBFW_ASSERT(ub_add_timer_task(timer, monitor_timer, NULL, 10000/*ms*/) == 0, 
				"start monitor callback");
		UBFW_ASSERT(ub_run_timer_task(timer) >0, "run timer error");


		monitor->run();
		ub_svr_vec_join(svrvec);	//?ȴ?server
		monitor->join(); 
		ub_svr_vec_destroy(svrvec);	//????server
		ub_join_timer_task(timer);	//?ȴ???ʱ??
		ub_destroy_timer_task(timer);	//???ٶ?ʱ??
		database_pool->destroy();
		ub_log_close();
		ubfw_close_galileo_log();

		ub_close(fw);
		return 0;
}
