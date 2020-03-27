#ifndef __WIN_SERVER_H__
#define __WIN_SERVER_H__

#include "../server.h"

class winServer : public server
{
public:
    winServer() { initServer(); }
    ~winServer();
protected:
    virtual int initServer();
    virtual void processAccept(mSOCKET clientSock, process pro);
};
#endif