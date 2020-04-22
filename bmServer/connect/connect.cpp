#include <mutex>
#include "connect.h"
#include "sql/data_operate.h"

namespace bm
{
namespace connect
{
    int func_data::func_call()
    {
        if (func_name == "create_account")
        {
            string phone    = vParams.at(0).pd;
            string name     = vParams.at(1).pd;
            string nickName = vParams.at(2).pd;
            string pwd      = vParams.at(3).pd;
            string id;
            int r = bm::data::create_user(phone, name, nickName, pwd, id);
            return r;
        }

        return -1;
    }
    std::mutex g_accept_lock;
    void decrypt(const string& from, string& to)
    {
        string dt;
        if (bm::decrypt_base64(from, dt, false))
            return;

        bm::decrypt_RSA(dt, to, "private.key", false);
    }
    void encrypt(const string& from, string& to)
    {
        string toEncrypt, toBase64;
        int r = bm::encrypt_RSA(from, toEncrypt, "public.key", true);
        cout << r << endl;

        r = bm::encrypt_base64(toEncrypt, toBase64, false);
        
        to = toBase64;
    }
    void accept_sock(const string& data, mSOCKET s)
    {
        std::lock_guard<std::mutex> lock(g_accept_lock);

        cout << "acceptSock:" << data << endl;
        closesocket(s);

        string drsa;
        decrypt(data, drsa);

        Json::Reader read;
        Json::Value root;
        if (!read.parse(drsa, root))
            return;
        func_data fd;
        pare_json(root, fd);
        fd.func_call();
    }
    void pare_json(const Json::Value& root, func_data& fd)
    {
        cout << root.toStyledString() << endl;
        string fname = root["function_name"].asString();
        fd.func_name = fname;

        int fnum = root["function_param_num"].asInt();
        fd.parm_num = fnum;

        Json::Value t = root["function_params"];
        for (auto i : t)
        {
            if (i.isNull())
                continue;

            int pt = i["param_type"].asInt();
            string pd = i["param_data"].asString();

            func_param fp;
            fp.pt = (param_type)pt;
            fp.pd = pd;
            fd.vParams.push_back(fp);
        }
    }
}
}