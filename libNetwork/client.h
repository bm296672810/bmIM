#ifndef __CLIENT_H__
#define __CLIENT_H__
#include "socket.h"

class client {
public:
    client(const string& ip, int port);
    int send(const string& data);
    int recv(string& data);
    bool read_data(unsigned char *recvData, int *resLength);

    bool is_connected() { return m_b_is_connected; }
    void set_connected(bool con) { m_b_is_connected = con; }
    bool get_connect_status() { return m_b_is_connected; }

    bool connect();
    bool dis_connect();

    void set_timeout(int readTimeOut, int writeTimeOut);

    ~client();
private:
    int socket_tcp_alive(SOCKET socket);
    SOCKET tcp_connect_server();
    void init_socket(const string& ip, int port);
private:
    SOCKET m_socket;
    Address* m_address;
    bool m_b_is_connected = false;

    int m_n_read_timeout = 2000;
    int m_n_write_timeout = 2000;

    int connect_status = -1;
};

#endif