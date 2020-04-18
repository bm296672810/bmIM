#ifndef __CONNECT_H__
#define __CONNECT_H__
#include "client.h"
#include "server.h"

#include "libOpenssl.h"
#include "json.h"

namespace bm
{
namespace connect
{
    enum paramType
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
    struct funcParam
    {
        paramType pt = _UNKNOW; // 参数类型
        string pd = "";         // 参数数据
    };
    struct funcData
    {
        string funcName;
        int parmNum = 0;
        vector<funcParam> vParams;
        int funcCall();
    };
    void decrypt(const string& from, string& to);
    void encrypt(const string& from, string& to);
    void acceptSock(const string& data, mSOCKET s);
    void pareJson(const Json::Value& root, funcData& fd);

}
}
#endif