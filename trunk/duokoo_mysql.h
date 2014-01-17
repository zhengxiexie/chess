/*
 * =====================================================================================
 *
 *       Filename:  duokoo_mysql.h
 *
 *    Description:  对mysql客户端函数进行简单封装
 *
 *        Version:  1.0
 *        Created:  xx/xx/2011 xx:xx:xx [A/P]M
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  RD. Li,Yong
 *          Email:  liyong04@baidu.com & liyong@duoku.com & linuxyong@gmail.com
 *        Company:  Baidu, Beijing, China
 *
 * =====================================================================================
 */
#ifndef _DUOKOO_MYSQL_H_
#define _DUOKOO_MYSQL_H_

#include "myclient_include.h"

extern MyclientPool * database_pool;
using namespace std;

class DuokooMysql
{
public:
    DuokooMysql(const char* db_name)
    {
        _count_num = 0;
        _err = 0;
        _p_data_res = NULL;
        memset( &_data_res, 0x00, sizeof( _data_res ) );
        _database_connection = database_pool->fetchConnection(db_name, &_err, 0);
        UB_LOG_NOTICE( "========mysql count[%d]========", ++_count );
    }

    DuokooMysql()
    {
        _count_num = 0;
        _err = 0;
        _p_data_res = NULL;
        memset( &_data_res, 0x00, sizeof( _data_res ) );
        _database_connection = database_pool->fetchConnection("default_mysql", &_err, 0);
        UB_LOG_NOTICE( "========mysql count[%d]========", ++_count );
    }
    DuokooMysql( int db  )
    {
        _count_num = 0;
        _err = 0;
        _p_data_res = NULL;
        memset( &_data_res, 0x00, sizeof( _data_res ) );
        if( db == 2 ){
            _database_connection = database_pool->fetchConnection("payment_mysql", &_err, 0);
        }else{
            _database_connection = database_pool->fetchConnection("default_mysql", &_err, 0);
        }
        UB_LOG_NOTICE( "========mysql count[%d]========", ++_count );
    }
    ~DuokooMysql()
    {
        if( _p_data_res != NULL ){
            _p_data_res->free();
        }
        _data_res.free();
        database_pool->putBackConnection( _database_connection, false);
        --_count;
    }
    const MysqlConnection *get_conn() const
    {
        return _database_connection;
    }
    const int get_err() const
    {
        return _err;
    }
    void set_query_result_buf( MyclientRes* data_res )
    {
        _p_data_res = data_res;
        memset( _p_data_res, 0x00, sizeof( MyclientRes ) );
    }
    int get_errno()
    {
        if(_database_connection==NULL){
            return -5;
        }
        return _database_connection->getErrno();
    }
    string getErrDes()
    {
        if(_database_connection==NULL){
            return  string();
        }
        return string(_database_connection->getErrnoDes());
    }
    int query( const string& query );
    int query_with_retry( const string& query, const uint32_t retry_times, const uint32_t interval = 1000 );

    int queryInTransaction( const vector<string>& sql_list );

    string getResult( const uint32_t row, const uint32_t col );
    string get_result( const uint32_t row, const uint32_t col );

    bool startTransaction();
    bool commit();
    bool rollback();
	string escapeString(const string& str);
private:
    MysqlConnection * _database_connection;
    string _query_sql;
    MyclientRes* _p_data_res;
    MyclientRes _data_res;
    int _count_num;
    int _err;
    static int32_t _count;
};

typedef DuokooMysql duokoo_mysql;
typedef DuokooMysql mcp_mysql;

#endif
