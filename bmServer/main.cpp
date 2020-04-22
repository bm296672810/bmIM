#include <iostream>
#include <fstream>

#include "lib_openssl.h"
#include "client.h"
#include "server.h"
#include "lib_sql.h"
#include "sql/data_operate.h"
#include "connect/connect.h"

#ifndef M_SLEEP
#include <thread>
#include <chrono>
#define M_SLEEP(t) std::this_thread::sleep_for(std::chrono::milliseconds(t))
#define U_SLEEP(t) std::this_thread::sleep_for(std::chrono::microseconds(t))
#endif

#ifdef _WIN32
#pragma comment(lib, "ws2_32.lib")
#endif

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
    //bm::makeRSAKey("private.key", "public.key", 1024);
    bm::sql::mysql::initConnectPool("bm", "bm12345", "192.168.183.129", 3306, 50);
    
    //string id;
    //int r = bm::data::createUser("17665319088", "name1", "bm1", "bm12345", id);
    server* s = new win_server();
    s->init_process(bm::connect::accept_sock);
    string from, to, dto;
    std::fstream f("functionData.txt");
    std::istreambuf_iterator<char> beg(f), end;
    from = string(beg, end);
    
    bm::connect::encrypt(from, to);
    //bm::connect::encrypt("hello world", to);
    bm::connect::decrypt(to, dto);
    while (true)
    {
        bool br = false;// sendData(ip, port, data);
        cout << "sendData:" << br << endl;

        M_SLEEP(5000);
    }

    bm::sql::mysql::releseConnectPool();
	return 0;
}