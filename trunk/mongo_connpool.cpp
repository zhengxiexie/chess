#include "mongo_connpool.h"

mongo_connpool * mongo_connpool::connPool = NULL;
bool mongo_connpool::b_lock = false;

//建立连接池
mongo_connpool::mongo_connpool(std::string host[], int port[], int maxSize)
{
        //设置最大连接数
	this->maxSize = maxSize;
        //当前连接数
	this->curSize = 0;

	for(int i=0; i< mongo_server_num; i++)
	{
		memset(this->host[i], 0x00, sizeof(this->host[i]));
		strcpy(this->host[i],host[i].c_str());
		this->port[i] = port[i];
                UB_LOG_NOTICE( "+++++++++ host[%s;%d] +++++++", this->host[i], this->port[i] );
	}
        //初始化线程互斥锁
	pthread_mutex_init(&locker, NULL);
        //
	this->init(maxSize/2);
}

mongo_connpool::~mongo_connpool()
{
        UB_LOG_NOTICE( "+++++ call_destroy +++++, [%s:%d]", __FILE__, __LINE__ );
	this->destroy();
        //销毁互斥锁
	pthread_mutex_destroy(&locker);
}

void mongo_connpool::lockMutex(bool &b_lock = mongo_connpool::b_lock)
{
	while(b_lock);
    b_lock = true;
}

void mongo_connpool::unlockMutex(bool &b_lock = mongo_connpool::b_lock)
{
	b_lock = false;
}

mongo_connpool *mongo_connpool::getInstance()
{
	if(connPool == NULL) //如果连接池未创建，创建连接池
	{
		lockMutex();  // 上锁
		if(connPool == NULL)  //创建连接池
			connPool = new mongo_connpool(mongo_host, mongo_port, mongo_server_max_size);
		unlockMutex(); //解锁
	}

	return connPool;
}

/*
void mongo_connpool::load_config(const)
{
}
*/

void mongo_connpool::init(int size)
{
	mongo* conn;
	for(int i = 0; i < size; )
	{
		conn = this->createConnection(); // 生成mongodb连接
		if(conn)
		{
			i++;
			conns.push_back(conn); // 放进连接池队列
			curSize++; //当前连接数+1
		}
		else
			UB_LOG_FATAL("init mongo connpool fail one\n");
	}
	UB_LOG_NOTICE("init mongo connpool complete total %d\n", curSize);
}

void mongo_connpool::destroy()
{
	bsl::deque<mongo*>::iterator pos;
	pthread_mutex_lock(&locker);   //上线程锁
	for(pos = conns.begin(); pos != conns.end(); ++pos)  //循环释放连接
		this->terminateConnection(*pos);

	curSize = 0; // 重置当前连接数
	conns.clear(); // 重置连接池
	pthread_mutex_unlock(&locker);  //解除线程锁
}

mongo* mongo_connpool::createConnection()
{
	mongo *conn;
	conn = (mongo*)malloc(sizeof(mongo));
	int i = 0;
/*
	mongo_replset_init( conn, "mcp" );
	mongo_replset_add_seed( conn, "10.23.245.118", 27030 );
	mongo_replset_add_seed( conn, "10.23.245.118", 27031 );
	mongo_replset_add_seed( conn, "10.23.245.118", 27032 );
        int status = mongo_replset_connect( conn );
        if( status != MONGO_OK )
        {
            UB_LOG_FATAL( "connecttion failed, err[%d], errstr[%s]", conn->err, conn->errstr );
            bson out[1];
            mongo_cmd_get_last_error( conn, "mcp", out );
            UB_LOG_FATAL( "connecttion failed, err[%d], errstr[%s]", conn->lasterrcode, conn->lasterrstr );
        }
        else
        {
            return conn;
        }
*/
	int status = mongo_connect( conn, host[i], port[i]);
	if( status != MONGO_OK )
	{
		if(conn->err != MONGO_CONN_SUCCESS)
			UB_LOG_FATAL( "connection failed host is %s", host[i]);
	}
	else
		return conn;
	return NULL;
}

void mongo_connpool::terminateConnection(mongo* conn)
{
	if(conn)
	{
		try
		{
			mongo_destroy( conn );
			if(conn!=NULL)
			{
				free(conn);
				conn = NULL;
			}
		}
		catch(...)
		{
			UB_LOG_FATAL("close mongo error\n");
		}
	}
}

mongo* mongo_connpool::getConnection()
{
	mongo* conn = NULL;
	pthread_mutex_lock(&locker);   // 上线程锁


        UB_LOG_NOTICE( "+++++++++++++ size[%d] +++++++++++++++++", (int)conns.size() );
	if(conns.size() > 0)   //have free connection
	{
		conn = conns.front();  // 取出连接
		conns.pop_front();   // 压出队列
		if( mongo_check_connection(conn) != MONGO_OK )
		{   // 连接已经不存在 
			UB_LOG_WARNING("a mongo conn has error %s", conn->errstr);
			mongo_destroy(conn);
			if(conn != NULL)
			{
				free(conn);
				conn = NULL;
			}
			conn = this->createConnection();  // 创建一个新连接
		}
		if(conn == NULL)
		{
			UB_LOG_WARNING("get a  mongo conn is null");
			--curSize;
		}
		pthread_mutex_unlock(&locker);  // 线程解锁
		return conn;
	}
	else
	{
		if(curSize < maxSize)  //can still create new connection
		{
			conn = this->createConnection();
			if(conn)
			{
				++curSize;
				pthread_mutex_unlock(&locker);
				return conn;
			}
			else
			{
				pthread_mutex_unlock(&locker);
				return NULL;
			}
		}
		else
		{
			pthread_mutex_unlock(&locker);
			return NULL;
		}
	}
}

void mongo_connpool::releaseConnection(mongo* conn)
{
	if(conn)
	{
		pthread_mutex_lock(&locker);
		conns.push_back(conn);
		pthread_mutex_unlock(&locker);
	}
}

mongo* mongo_connpool::getConnectionTry(int maxNum)
{
	mongo *conn;
	for(int i = 0; i < maxNum; ++i)
	{
		conn = this->getConnection();
		if(conn)
			return conn;
		else
			sleep(2);
	}
	return NULL;
}
