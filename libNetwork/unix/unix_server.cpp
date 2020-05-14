#include "unix_server.h"
#include <thread>
#include <memory>
#include <string>
#include <iostream>

#include <WinSock2.h>
#include<Ws2tcpip.h>

win_server::~win_server()
{
    s_baccept_thread = false;

    closesocket(m_socket);   //关闭套接字
    WSACleanup();            //释放套接字资源;
}
int win_server::init_server()
{
    //int    listenfd, connfd;
    struct sockaddr_in     servaddr;
    char    buff[4096];
    int     n;

    if ((m_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        printf("create socket error: %s(errno: %d)\n", strerror(errno), errno);
        return sock_error::createSocket_error;
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(m_port);

    if (bind(listenfd, (struct sockaddr*) & servaddr, sizeof(servaddr)) == -1) {
        printf("bind socket error: %s(errno: %d)\n", strerror(errno), errno);
        return sock_error::bind_error;
    }

    if (listen(listenfd, 10) == -1) {
        printf("listen socket error: %s(errno: %d)\n", strerror(errno), errno);
        return sock_error::listen_error;
    }
    //WSADATA wsd;
    //if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
    //    return sock_error::init_error;

    //m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    //if (INVALID_SOCKET == m_socket)
    //{
    //    WSACleanup();//释放套接字资源;
    //    return sock_error::createSocket_error;
    //}

    //struct sockaddr_in* addr = (struct sockaddr_in*) malloc(sizeof(struct sockaddr_in));
    //if (addr)
    //{
    //    addr->sin_family = AF_INET;
    //    addr->sin_addr.S_un.S_addr = INADDR_ANY;
    //    //inet_pton(addr->sin_family, INADDR_ANY, &(addr->sin_addr.S_un.S_addr));
    //    addr->sin_port = htons((unsigned short)m_port);
    //}
    //else
    //    return sock_error::addr_error;

    //int r = bind(m_socket, (LPSOCKADDR)addr, sizeof(SOCKADDR_IN));
    //if (SOCKET_ERROR == r)
    //{
    //    closesocket(m_socket);   //关闭套接字
    //    WSACleanup();            //释放套接字资源;
    //    return sock_error::bind_error;
    //}

    //r = listen(m_socket, 1);
    //if (SOCKET_ERROR == r)
    //{
    //    closesocket(m_socket);   //关闭套接字
    //    WSACleanup();            //释放套接字资源;
    //    return sock_error::listen_error;
    //}

    std::shared_ptr<std::thread> t(new std::thread([&] {
        while (s_baccept_thread)
        {
            SOCKET sClient = INVALID_SOCKET;
            sockaddr_in addrClient;
            int addrClientlen = sizeof(addrClient);
            sClient = accept(m_socket, (sockaddr FAR*) & addrClient, &addrClientlen);
            std::cout << "accept:" << sClient << std::endl;
            if (INVALID_SOCKET == sClient)
            {

                break;
            }
            if (s_baccept_thread)
                process_accept(sClient, m_process);
        }
    }));

    t->detach();

    return sock_error::success;
}
void win_server::process_accept(mSOCKET clientSock, process pro)
{
    std::shared_ptr<std::thread> s(new std::thread([&] {
        process tmpPro = pro;
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

        if (tmpPro)
            tmpPro(result, clientSock);
        //send(tmpClient, result.c_str(), (int)result.length(), 0);

        //closesocket(tmpClient);
        //std::cout << result << std::endl;
    }));

    s->detach();
}