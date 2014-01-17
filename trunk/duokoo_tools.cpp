#include "duokoo_tools.h"
#include <openssl/md5.h>
#include <openssl/des.h>
#include <sys/time.h>
#include <math.h>
#include "ub_server.h"

using namespace std;
using namespace DuokooTools;

string DuokooTools::MD5(const string& src )
{
    MD5_CTX ctx;
    
    string md5_string;
    unsigned char md[16] = { 0 };
    char tmp[33] = { 0 };
        
    MD5_Init( &ctx );
    MD5_Update( &ctx, src.c_str(), src.size() );
    MD5_Final( md, &ctx );
        
    for( int i = 0; i < 16; ++i )
    {   
        memset( tmp, 0x00, sizeof( tmp ) );
        sprintf( tmp, "%02X", md[i] );
        md5_string += tmp;
    }
    return md5_string;
}

string DuokooTools::DES(const string& str)
{
    string des_string;
    try{
        string keystring( "duoko0@baidu.com" );
        DES_cblock key[1];
        DES_key_schedule key_schedule;

        DES_string_to_key( keystring.c_str(), key );
        if( DES_set_key_checked( key, &key_schedule ) != 0 ){
            throw "DES_set_key_schedule";
        }

        unsigned char input[64];
        memset( input, 0x00, sizeof( input) );
        strcpy( (char*)input, str.c_str() );
        size_t len = (sizeof(input)+7)/8 *8;
        unsigned char output[128];
        memset( output, 0x00, sizeof( output) );
        DES_cblock ivec;

        memset( (char*)&ivec, 0, sizeof(ivec));
        DES_ncbc_encrypt( input, output, sizeof( input ), &key_schedule, &ivec, 1 );

        char tmp[3];
        for( unsigned int i = 0; i < len; ++i ){
            memset( tmp, 0x00, sizeof( tmp ) );
            sprintf( tmp, "%02X", output[i] );
            des_string.append( tmp );
        }
    }catch( const char* errstr ){
        //UB_LOG_WARNING( "%s failed, [%s:%d]", errstr, __FILE__, __LINE__ );
        return string();
    }catch(...){
        //UB_LOG_WARNING( "unknow exception failed, [%s:%d]", __FILE__, __LINE__ );
    }
    return des_string;
}

string DuokooTools::date(const string& format, time_t t)
{
    struct tm *v_tm;
    t=(t==0?time(0):t);
    v_tm = localtime(&t);
    string time_str;

    size_t start_pos=0;
    size_t end_pos  =0;
    char c;
    do{
        end_pos=format.find('%',start_pos);
        time_str.append(format.substr(start_pos, end_pos-start_pos));
        if(end_pos+1<format.size()){
            c=format[end_pos+1];
            switch(c){
                case 'd':  //day of month (01..31)
                    time_str.append(toString(v_tm->tm_mday,2));
                    break;
                case 'D':  //date (mm/dd/yy)
                    time_str.append(toString(v_tm->tm_mon+1,2)).append("/")
                            .append(toString(v_tm->tm_mday,2)).append("/")
                            .append(toString(v_tm->tm_year%100,2));
                    break;
                case 'e':  //day of month, blank padded ( 1..31)
                    time_str.append(" ").append(toString(v_tm->tm_mday,2));
                    break;
                case 'F':  //same as %Y-%m-%d
                    time_str.append(toString(v_tm->tm_year+1900,4)).append("-")
                            .append(toString(v_tm->tm_mon+1,2)).append("-")
                            .append(toString(v_tm->tm_mday,2));
                    break;
                case 'H':  //hour (00..23)
                    time_str.append(toString(v_tm->tm_hour,2));
                    break;
                case 'k':  //hour ( 0..23)
                    time_str.append(" ").append(toString(v_tm->tm_hour,2));
                    break;
                case 'm':  //month (01..12)
                    time_str.append(toString(v_tm->tm_mon+1,2));
                    break;
                case 'M':  //minute (00..59)
                    time_str.append(toString(v_tm->tm_min,2));
                    break;
                case 'R':  //time, 24-hour (hh:mm)
                    time_str.append(toString(v_tm->tm_hour,2)).append(":")
                            .append(toString(v_tm->tm_min,2));
                    break;
                case 'S':  //second (00..60); the 60 is necessary to accommodate a leap second
                    time_str.append(toString(v_tm->tm_sec,2));
                    break;
                case 'T':  //time, 24-hour (hh:mm:ss)
                    time_str.append(toString(v_tm->tm_hour,2)).append(":")
                            .append(toString(v_tm->tm_min,2)).append(":")
                            .append(toString(v_tm->tm_sec,2));
                    break;
                case 'w': //day of week (0..6);  0 represents Sunday
                    time_str.append(toString(v_tm->tm_wday));
                    break;
                case 'x':  //locale's date representation (mm/dd/yy)
                    time_str.append(toString(v_tm->tm_mon+1,2)).append("/")
                            .append(toString(v_tm->tm_mday,2)).append("/")
                            .append(toString(v_tm->tm_year%100,2));
                    break;
                case 'X':  //locale's time representation (%H:%M:%S)
                    time_str.append(toString(v_tm->tm_hour,2)).append(":")
                            .append(toString(v_tm->tm_min,2)).append(":")
                            .append(toString(v_tm->tm_sec,2));
                    break;
                case 'y':  //last two digits of year (00..99)
                    time_str.append(toString(v_tm->tm_year%100,2));
                    break;
                case 'Y':  //year (1970...)
                    time_str.append(toString(v_tm->tm_year+1900,4));
                    break;
                default:
                    break;
            }
        }
        start_pos = end_pos+2;
    }while(end_pos<format.size());
    return time_str;
}

long DuokooTools::getRandomNum(const long range)
{
    struct timeval tv;
    gettimeofday( &tv, NULL );
    srandom( (uint32_t)tv.tv_usec );
    return long(range*double(random())/(RAND_MAX-1));
}

string DuokooTools::getRandomStr(const uint32_t length)
{
    return toString(getRandomNum((long)pow(10,length)), length);
}

int DuokooTools::YEAR()
{
    time_t t;
    struct tm* tm;
    t = time( 0 );
    tm = localtime( &t );
    return tm->tm_year+1900;
}

int DuokooTools::MONTH()
{
    time_t t;
    struct tm* tm;
    t = time( 0 );
    tm = localtime( &t );
    return tm->tm_mon+1;
}

string DuokooTools::NOW()
{
    return date("%F %T");
}

string DuokooTools::timestampToString(const time_t ts)
{
    //UB_LOG_TRACE( "DuokooTools::timestampToString start" );
    struct tm *t; 
    t = localtime(&ts);
    char time_string[32];
    
    stringstream ss; 
    ss.unsetf(ios::skipws);
    //UB_LOG_DEBUG("%d-%d-%d %d:%d:%d",t->tm_year+1990,t->tm_mon+1,t->tm_mday,t->tm_hour,t->tm_mon,t->tm_sec);
    ss<<setw(4)<<setfill('0')<<t->tm_year+1900<<"-"
      <<setw(2)<<setfill('0')<<t->tm_mon+1<<"-"
      <<setw(2)<<setfill('0')<<t->tm_mday<<" "
      <<setw(2)<<setfill('0')<<t->tm_hour<<":"
      <<setw(2)<<setfill('0')<<t->tm_min<<":"
      <<setw(2)<<setfill('0')<<t->tm_sec;
    ss.getline(time_string, 32);
    //UB_LOG_TRACE( "DuokooTools::timestampToString end" );
    return string(time_string);
}

time_t DuokooTools::stringToTimestamp(const string& s, const string& format)
{
    struct tm tm;
    strptime( s.c_str(), format.c_str(), &tm );
    time_t t;
    t = mktime(&tm);
    return t;
}
