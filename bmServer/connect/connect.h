#ifndef __CONNECT_H__
#define __CONNECT_H__
//#include "client.h"
#include <string>
#include "server.h"

#include "lib_openssl.h"
#include "json.h"
using namespace std;
namespace bm
{
namespace connect
{
    enum class param_type
    {
        _BOOL,
        _CHAR,
        _UCHAR,
        _SHORT,
        _USHORT,
        _INT,
        _UINT,
        _LONG,
        _ULONG,
        _FLOAT,
        _DOUBLE,
        _STRING,
        _UNKNOW
    };
    struct func_param
    {
        param_type pt = param_type::_UNKNOW; // 参数类型
        string pd = "";         // 参数数据
    };
    struct func_data
    {
        string func_name;
        int parm_num = 0;
        vector<func_param> vParams;
        int func_call();
    };
    void decrypt(const string& from, string& to);
    void encrypt(const string& from, string& to);
    void accept_sock(const string& data, mSOCKET s);
    void pare_json(const Json::Value& root, func_data& fd);

}
}
#endif