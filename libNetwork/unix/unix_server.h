#ifndef __UNIX_SERVER_H__
#define __UNIX_SERVER_H__
#include "../server.h"

class unix_server : public server
{
public:
    unix_server() { init_server(); }
    ~unix_server();
protected:
    virtual int init_server();
    virtual void process_accept(mSOCKET clientSock, process pro);
};
#endif