#ifndef __SERVER_H__
#define __SERVER_H__

#include <string>

#define BUF_SIZE 1024
#ifdef _WIN32
typedef unsigned long long mSOCKET;
#elif APPLE
#include "mac/macServer.h"
elif linux
#include "unix/unixServer.h"
#endif

enum sock_error
{
    accept_error         = -7,
    listen_error         = -6,
    bind_error           = -5,
    addr_error           = -4,
    createSocket_error   = -3,
    init_error           = -2,
    connect_error        = -1,
    success             = 0,
};

typedef void (*process)(const std::string& data, mSOCKET s);

static bool s_baccept_thread = true;
class server
{
public:
    server() {}
    ~server() {}
    void init_process(process pro)
    {
        if (pro)
            m_process = pro;
    }

protected:
    virtual int init_server() = 0;
    virtual void process_accept(mSOCKET clientSock, process pro) = 0;
    int m_port = 1122;
    mSOCKET m_socket;
    process m_process;
};

#ifdef _WIN32
#include "win/win_server.h"
#elif APPLE
#include "mac/mac_server.h"
elif linux
#include "unix/unix_server.h"
#endif
#endif