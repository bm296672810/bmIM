#ifndef __WIN_SERVER_H__
#define __WIN_SERVER_H__

#include "../server.h"

class win_server : public server
{
public:
    win_server() { init_server(); }
    ~win_server();
protected:
    virtual int init_server();
    virtual void process_accept(mSOCKET clientSock, process pro);
};
#endif