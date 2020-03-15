#include "winServer.h"

#include <thread>
#include <memory>
#include <string>
#include <iostream>

#include <WinSock2.h>
#include<Ws2tcpip.h>

winServer::~winServer()
{
    s_bAcceptThread = false;

    closesocket(m_socket);   //关闭套接字
    WSACleanup();            //释放套接字资源;
}
int winServer::initServer()
{
    WSADATA wsd;
    if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
        return sockError::initError;

    m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (INVALID_SOCKET == m_socket)
    {
        WSACleanup();//释放套接字资源;
        return sockError::createSocketError;
    }

    struct sockaddr_in* addr = (struct sockaddr_in*) malloc(sizeof(struct sockaddr_in));
    if (addr)
    {
        addr->sin_family = AF_INET;
        addr->sin_addr.S_un.S_addr = INADDR_ANY;
        //inet_pton(addr->sin_family, INADDR_ANY, &(addr->sin_addr.S_un.S_addr));
        addr->sin_port = htons((unsigned short)m_port);
    }
    else
        return sockError::addrError;

    int r = bind(m_socket, (LPSOCKADDR)addr, sizeof(SOCKADDR_IN));
    if (SOCKET_ERROR == r)
    {
        closesocket(m_socket);   //关闭套接字
        WSACleanup();            //释放套接字资源;
        return sockError::bindError;
    }

    r = listen(m_socket, 1);
    if (SOCKET_ERROR == r)
    {
        closesocket(m_socket);   //关闭套接字
        WSACleanup();            //释放套接字资源;
        return sockError::listenError;
    }

    std::shared_ptr<std::thread> t(new std::thread([&] {
        while (s_bAcceptThread)
        {
            SOCKET sClient = INVALID_SOCKET;
            sockaddr_in addrClient;
            int addrClientlen = sizeof(addrClient);
            sClient = accept(m_socket, (sockaddr FAR*) & addrClient, &addrClientlen);
            std::cout << "accept:" << sClient << std::endl;
            if (INVALID_SOCKET == sClient)
            {
                closesocket(m_socket);   //关闭套接字
                WSACleanup();            //释放套接字资源;
                break;
            }
            if(s_bAcceptThread)
                processAccept(sClient);
        }
    }));
    
    t->detach();

    return sockError::success;
}
void winServer::processAccept(mSOCKET clientSock)
{
    std::shared_ptr<std::thread> s(new std::thread([&] {
        SOCKET tmpClient = clientSock;
        char* buf = new char[BUF_SIZE + 1];
        memset(buf, 0, BUF_SIZE + 1);
        int r = 0;
        std::string result;
        do
        {
            r = recv(tmpClient, buf, BUF_SIZE, 0);
            result += buf;
            std::cout << "recv size:" << r << std::endl;
        } while (r >= BUF_SIZE);

        send(tmpClient, result.c_str(), (int)result.length(), 0);

        closesocket(tmpClient);
        std::cout << result << std::endl;
    }));

    s->detach();
}