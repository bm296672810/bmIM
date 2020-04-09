#include <mutex>
#include "connect.h"

namespace bm
{
namespace connect
{
    std::mutex g_acceptLock;

    void acceptSock(const string& data, mSOCKET s)
    {
        std::lock_guard<std::mutex> lock(g_acceptLock);

        cout << "acceptSock:" << data << endl;
        closesocket(s);
        string dt;
        if (!bm::decryptBase64(data, dt, false))
            return;

        string drsa;
        if (!bm::decryptRSA(dt, drsa, "private.key"))
            return;

        Json::Reader read;
        Json::Value root;
        if (!read.parse(drsa, root))
            return;

        pareJson(root);
    }
    void pareJson(const Json::Value& root)
    {
        cout << root.toStyledString() << endl;
        string fname = root["function_name"].asString();
        if (fname == "create_account")
        {

        }
    }
}
}