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

enum sockError
{
    acceptError         = -7,
    listenError         = -6,
    bindError           = -5,
    addrError           = -4,
    createSocketError   = -3,
    initError           = -2,
    connectError        = -1,
    success             = 0,
};

typedef void (*process)(const std::string& data, mSOCKET s);

static bool s_bAcceptThread = true;
class server
{
public:
    server() {}
    ~server() {}
    void initProcess(process pro)
    {
        if (pro)
            m_process = pro;
    }

protected:
    virtual int initServer() = 0;
    virtual void processAccept(mSOCKET clientSock, process pro) = 0;
    int m_port = 1122;
    mSOCKET m_socket;
    process m_process;
};

#ifdef _WIN32
#include "win/winServer.h"
#elif APPLE
#include "mac/macServer.h"
elif linux
#include "unix/unixServer.h"
#endif
#endif