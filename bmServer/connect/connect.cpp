#include <mutex>
#include "connect.h"
#include "sql/dataOperate.h"

namespace bm
{
namespace connect
{
    int funcData::funcCall()
    {
        if (funcName == "create_account")
        {
            string phone = vParams.at(0).pd;
            string name = vParams.at(1).pd;
            string nickName = vParams.at(2).pd;
            string pwd = vParams.at(3).pd;
            string id;
            int r = bm::data::createUser(phone, name, nickName, pwd, id);
            return r;
        }

        return -1;
    }
    std::mutex g_acceptLock;
    void decrypt(const string& from, string& to)
    {
        string dt;
        if (bm::decryptBase64(from, dt, false))
            return;

        bm::decryptRSA(dt, to, "private.key");
    }
    void encrypt(const string& from, string& to)
    {
        string toEncrypt, toBase64;
        int r = bm::encryptRSA(from, toEncrypt, "public.key");
        cout << r << endl;

        r = bm::encryptBase64(toEncrypt, toBase64, false);
        
        to = toBase64;
    }
    void acceptSock(const string& data, mSOCKET s)
    {
        std::lock_guard<std::mutex> lock(g_acceptLock);

        cout << "acceptSock:" << data << endl;
        closesocket(s);

        string drsa;
        decrypt(data, drsa);

        Json::Reader read;
        Json::Value root;
        if (!read.parse(drsa, root))
            return;
        funcData fd;
        pareJson(root, fd);
        fd.funcCall();
    }
    void pareJson(const Json::Value& root, funcData& fd)
    {
        cout << root.toStyledString() << endl;
        string fname = root["function_name"].asString();
        fd.funcName = fname;

        int fnum = root["function_param_num"].asInt();
        fd.parmNum = fnum;

        Json::Value t = root["function_params"];
        for (auto i : t)
        {
            if (i.isNull())
                continue;

            int pt = i["param_type"].asInt();
            string pd = i["param_data"].asString();

            funcParam fp;
            fp.pt = (paramType)pt;
            fp.pd = pd;
            fd.vParams.push_back(fp);
        }
    }
}
}