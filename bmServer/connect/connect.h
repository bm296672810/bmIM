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
    void acceptSock(const string& data, mSOCKET s);
    void pareJson(const Json::Value& root);
}
}
#endif