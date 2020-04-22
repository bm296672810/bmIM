#include <iostream>

#include "lib_openssl.h"
#include "client.h"
#include "server.h"
#include "lib_sql.h"

#ifndef M_SLEEP
#include <thread>
#include <chrono>
#define M_SLEEP(t) std::this_thread::sleep_for(std::chrono::milliseconds(t))
#define U_SLEEP(t) std::this_thread::sleep_for(std::chrono::microseconds(t))
#endif
#pragma comment(lib, "ws2_32.lib")
using namespace std;
bool send_data(const string& ip, int port, const string& data)
{
    string toEncrypt;
    int r = bm::encrypt_RSA(data, toEncrypt, "public.key", true);
    cout << r << endl;

    string toBase64;
    r = bm::encrypt_base64(toEncrypt, toBase64, false);
    cout << r << endl;

    client c(ip, port);
    c.connect();
    if (c.is_connected())
    {
        if (c.send(toBase64) != toBase64.length())
            return false;
    }
    else
        return false;

    return true;
}
int main(int argc, char *argv[])
{
    string data = "Hello World!";

    string toBase64, toSource, to;

    toBase64 = "Bzld+VONPDNV7GjcFss24K9zWiarJG4+WqKSkT+3CSNL3XTBQN2IBr6cv8pr9d/WlJaaz7ugyYyZv1SG0U8BEBYNw+1ztjZZd8ZR8Dz7HMvb4tdma7O9Zu+gI10cCzy6Jwy4ll2n6IV2dfKAva5MivLAK4pvEUIMUIGuUFul32E=";
    int r = bm::decrypt_base64(toBase64, toSource, false);

    r = bm::decrypt_RSA(toSource, to, "private.key", false);

    string ip = "192.168.2.175";
    int port = 10011;

    server* s = new win_server();
    
    //bm::sql::mysql::SqlOperate op("bm", "bm12345", "192.168.183.129");
    bm::sql::SqlResult result;
    string sql = "insert into t_users values(NULL, 'bm1234567', 'bmTest', '17665319084', '296672810@qq.com', NULL, CURRENT_TIMESTAMP, 0, CURRENT_TIMESTAMP);";
    //unsigned int er = op.ExecSql(sql, result);
    int er = bm::sql::mysql::initConnectPool("bm", "bm12345", "192.168.183.129", 3306, 50);
        er = bm::sql::mysql::execSql(sql, result);
    string sql1 = "select * from t_users";
    for (int i = 0; i < 500; i++)
    {
        (new thread([&]() {
            string sql2 = sql1;
            bm::sql::SqlResult result;
            while (true)
            {
                int e = bm::sql::mysql::execSql(sql2, result);

                cout << "thread id:" << std::this_thread::get_id() << " execSql result:" << e << endl;

                M_SLEEP(1000);
            }
        }))->detach();
    }
    while (true)
    {
        bool br = false;// sendData(ip, port, data);
        cout << "sendData:" << br << endl;

        M_SLEEP(5000);
    }

    bm::sql::mysql::releseConnectPool();
	return 0;
}