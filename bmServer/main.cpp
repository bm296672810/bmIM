#include <iostream>
#include <fstream>

#include "libOpenssl.h"
#include "client.h"
#include "server.h"
#include "libSql.h"
#include "sql/dataOperate.h"
#include "connect/connect.h"

#ifndef M_SLEEP
#include <thread>
#include <chrono>
#define M_SLEEP(t) std::this_thread::sleep_for(std::chrono::milliseconds(t))
#define U_SLEEP(t) std::this_thread::sleep_for(std::chrono::microseconds(t))
#endif
#pragma comment(lib, "ws2_32.lib")
using namespace std;
bool sendData(const string& ip, int port, const string& data)
{
    string toEncrypt;
    int r = bm::encryptRSA(data, toEncrypt, "public.key");
    cout << r << endl;

    string toBase64;
    r = bm::encryptBase64(toEncrypt, toBase64, false);
    cout << r << endl;

    Client c(ip, port);
    c.Connect();
    if (c.IsConnected())
    {
        if (c.Send(toBase64) != toBase64.length())
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
    server* s = new winServer();
    s->initProcess(bm::connect::acceptSock);
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