#ifndef __SOCKET_H__
#define __SOCKET_H__
#include <string>

#ifdef WIN32
#   include <winsock2.h>
#   include<Ws2tcpip.h>
#include <mstcpip.h>
#else
#   include <netinet/in.h>
#   include<arpa/inet.h>
#endif

using namespace std;

class Address{
public:
    Address() { m_iPort = -1; m_strIp = ""; }
    Address(const string& ip, int port)
    {
        m_strIp = ip;
        m_iPort = port;
    }
    string m_strIp;
    int m_iPort;
};
struct sockaddr_in* to_sockaddr(Address* a);

Address* from_sockaddr(struct sockaddr_in* address);

//class Socket {
//public:
//    Socket(const string& ip, int port);
//    int bind();
//    int listen();
//    int accept();
//    int recv(string& data);
//    int send(const string& data);
//    int connect();
//    int disConnect();
//    ~Socket();
//private:
//    SOCKET m_tcpsocket;
//    Address* m_address;
//    struct sockaddr_in* m_saddr;
//};
#endif