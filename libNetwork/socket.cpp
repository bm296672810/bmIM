#include "socket.h"

struct sockaddr_in* to_sockaddr(Address* a)
{
    struct sockaddr_in* ret = (struct sockaddr_in*) malloc(sizeof(struct sockaddr_in));

    if (ret)
    {

        ret->sin_family = AF_INET;
#ifdef _WIN32
        inet_pton(ret->sin_family, a->m_strIp.c_str(), &(ret->sin_addr.S_un.S_addr));
#elif linux
        inet_pton(ret->sin_family, a->m_strIp.c_str(), &(ret->sin_addr));
#endif
        ret->sin_port = htons((unsigned short)a->m_iPort);
    }

    return ret;
}
Address* from_sockaddr(struct sockaddr_in* address)
{
    Address* ret = new Address;

    char tmp[20];
    inet_ntop(address->sin_family, &address->sin_addr, tmp, 20);

    ret->m_strIp = tmp;
    ret->m_iPort = ntohs(address->sin_port);

    return ret;
}