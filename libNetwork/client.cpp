#include <thread>
#include <chrono>
#include <assert.h>
#include <iostream>

#include "client.h"

client::client(const string& ip, int port)
{
    init_socket(ip, port);
}
void client::init_socket(const string& ip, int port)
{
    WORD sockVersion = MAKEWORD(2, 2);
    WSADATA data;
    if (WSAStartup(sockVersion, &data) != 0)
    {
        return;
    }

    //m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    m_address = new Address(ip, port);
}
int client::send(const string& data)
{
    return ::send(m_socket, data.c_str(), (int)data.length(), 0);
}

int client::recv(string& data)
{
    int bufLen = 10, recvLen = 0;
    char* buf = new char[bufLen + 1];
    do {
        memset(buf, 0, bufLen + 1);
        recvLen = ::recv(m_socket, buf, bufLen + 1, 0);
        if(recvLen > 0)
            data += string(buf, recvLen);
    } while (recvLen >= bufLen);

    return (int)data.length();
}

int client::socket_tcp_alive(SOCKET socket)
{
    int ret = 0;

    int keep_alive = 1;
    ret = setsockopt(socket, SOL_SOCKET, SO_KEEPALIVE, (char*)&keep_alive, sizeof(keep_alive));

    if (ret == SOCKET_ERROR)
    {
        printf("setsockopt failed: %d \n", WSAGetLastError());
        return -1;
    }

    struct tcp_keepalive in_keep_alive = { 0 };
    unsigned long ul_in_len = sizeof(struct tcp_keepalive);
    struct tcp_keepalive out_keep_alive = { 0 };
    unsigned long ul_out_len = sizeof(struct tcp_keepalive);
    unsigned long ul_bytes_return = 0;

    in_keep_alive.onoff = 1;                    /*打开keepalive*/
    in_keep_alive.keepaliveinterval = 5000;     /*发送keepalive心跳时间间隔-单位为毫秒*/
    in_keep_alive.keepalivetime = 1000;         /*多长时间没有报文开始发送keepalive心跳包-单位为毫秒*/

    ret = WSAIoctl(socket, SIO_KEEPALIVE_VALS, (LPVOID)&in_keep_alive, ul_in_len,
        (LPVOID)&out_keep_alive, ul_out_len, &ul_bytes_return, NULL, NULL);

    if (ret == SOCKET_ERROR)
    {
        printf("WSAIoctl failed: %d \n", WSAGetLastError());
        return -1;
    }

    return 0;
}
SOCKET client::tcp_connect_server()
{
    connect_status = 0;
    
    sockaddr_in* serverAddr = to_sockaddr(m_address);
    m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_socket == -1)
        return INVALID_SOCKET;

    connect_status = ::connect(m_socket, (struct sockaddr*)serverAddr, sizeof(*serverAddr));
    if (connect_status == SOCKET_ERROR)
    {
        int errNO = WSAGetLastError();
        printf("connect errorNo:%d !\n", errNO);
        closesocket(m_socket);

        m_b_is_connected = false;
    }
    else
        m_b_is_connected = true;

    return m_socket;
}

bool client::connect()
{
    if (m_b_is_connected)
        return true;

    if (tcp_connect_server() && connect_status != -1)
    {
        socket_tcp_alive(m_socket);
        m_b_is_connected = true;

        set_timeout(m_n_read_timeout, m_n_write_timeout);
    }
    else
        m_b_is_connected = false;

    return m_b_is_connected;
}
bool client::dis_connect()
{
    closesocket(m_socket);
    delete m_address;
    m_address = NULL;

    return true;
}
void client::set_timeout(int readTimeOut, int writeTimeOut)
{
    m_n_read_timeout = readTimeOut;
    m_n_write_timeout = writeTimeOut;
    setsockopt(m_socket, SOL_SOCKET, SO_SNDTIMEO, (char *)&m_n_read_timeout, sizeof(int));
    setsockopt(m_socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&m_n_write_timeout, sizeof(int));
}

client::~client()
{
    dis_connect();
}
bool client::read_data(unsigned char *recvData, int *resLength)
{
    int nRes;
    //assert(m_bIsConnected);
    if (!m_b_is_connected)
        return false;
    nRes = ::recv(m_socket, (char*)recvData, 2048, 0);

    if (nRes == SOCKET_ERROR)
    {
        closesocket(m_socket);
        m_b_is_connected = false;
        return false;
    }
    else {
        *resLength = nRes;
        return true;
    }
}