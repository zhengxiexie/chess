/*
 * =====================================================================================
 *
 *       Filename:  duokoo_mysql.cpp
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
#include "duokoo_mysql.h"

int32_t DuokooMysql::_count=0;

int DuokooMysql::query( const string& sql )
{       
    if(_database_connection==NULL){
        UB_LOG_FATAL( "database_connection is NULL, [%s:%d]", __FILE__, __LINE__ );
        return -5;
    }
    if( _p_data_res==NULL ){
        _p_data_res = &_data_res;
    }
    _query_sql = sql;
    _p_data_res->free();
    memset( _p_data_res, 0x00, sizeof( MyclientRes ) );
    _count_num = _database_connection->query( _query_sql.c_str(), _p_data_res, true );
    return _count_num;
}

int DuokooMysql::query_with_retry( const string& sql, const uint32_t retry_times, const uint32_t interval )
{
    _count_num = 0;
    for( uint32_t i = 0; i < retry_times; ++i ){
        UB_LOG_NOTICE( "retry_times[%d], sql[%s], [%s:%d]", i, sql.c_str(), __FILE__, __LINE__ );
        _count_num = this->query( sql );
        if( _count_num>0 ){
            return _count_num;
        }
        usleep( interval );
    }
    return _count_num;
}

int DuokooMysql::queryInTransaction( const vector<string>& sql_list )
{
    int res(0);
    if(_database_connection==NULL){
        UB_LOG_FATAL( "get a null connection, [%s:%d]", __FILE__, __LINE__ );
        return -5;
    }
    const char **sql_buf = new const char*[sql_list.size()];
    for(size_t i=0; i<sql_list.size(); ++i){
       sql_buf[i] = sql_list[i].c_str();
    }
    res = _database_connection->queryInTransaction( sql_buf, sql_list.size() );
    if( res!=0 ){
        UB_LOG_FATAL( "queryInTransaction failed, [%s:%d]", __FILE__, __LINE__ );
    }
    delete[] sql_buf;
    return res;
}

string DuokooMysql::getResult( const uint32_t row, const uint32_t col )
{
    if( _p_data_res == NULL ){
        return string();
    }
    if( (*_p_data_res)[row][col] == NULL ){
        return string();
    }
    return string( (*_p_data_res)[row][col] );
}

string DuokooMysql::get_result( const uint32_t row, const uint32_t col )
{
    return this->getResult(row,col);
}

bool DuokooMysql::startTransaction()
{
    if(_database_connection==NULL){
        return false;
    }
    return _database_connection->startTransaction();
}

bool DuokooMysql::commit()
{
    return _database_connection->commit();
}

bool DuokooMysql::rollback()
{
    return _database_connection->rollback();
}

string DuokooMysql::escapeString(const string& str)
{
	if(_database_connection == NULL){
		return str;

	}   
	char* buf = new char[str.size()*2+1];
	int ret = _database_connection->escapeString(buf, str.c_str(), str.size()*2+1, str.size());
	if(ret<0){
		UB_LOG_FATAL( "string escape failed, str[%s]", str.c_str() );
		delete[] buf;
		return string();

	}   
	string escape_str(buf);
	delete[] buf;
	return escape_str;
}
