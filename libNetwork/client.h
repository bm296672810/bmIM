#ifndef __CLIENT_H__
#define __CLIENT_H__
#include "socket.h"

class Client {
public:
    Client(const string& ip, int port);
    int Send(const string& data);
    int Recv(string& data);
    bool ReadData(unsigned char *recvData, int readLength, int *resLength);

    bool IsConnected() { return m_bIsConnected; }
    void SetConnected(bool con) { m_bIsConnected = con; }
    bool GetConnectStatus() { return m_bIsConnected; }

    bool Connect();
    bool DisConnect();

    void SetTimeOut(int readTimeOut, int writeTimeOut);

    ~Client();
private:
    int socketTcpAlive(SOCKET socket);
    SOCKET tcpConnectServer();
    void InitSocket(const string& ip, int port);
private:
    SOCKET m_socket;
    Address* m_address;
    bool m_bIsConnected = false;

    int m_nReadTimeOut = 2000;
    int m_nWriteTimeOut = 2000;

    int connectStatus = -1;
};

namespace udp {
    SOCKET _udpClientInit(const char* ip, int port, sockaddr** addr);
    void _udpSocketClose(SOCKET socket);
    SOCKET _udpServerInit(const char* ip, int port, sockaddr** addr);
    int _udpSend(const char* ip, int port, const char* bufSend);
    int _udpRecv(const char* ip, int port, char** result);

    double refitySendData(const string& ip, int port, const string& currentIp, int currentPort, bool* isConnect);
	bool refitySetGuideManualData(const string& ip, int port);
	bool refitySetGuideAutoData(const string& ip, int port);
	bool refitySetGuideCenterData(const string& ip, int port);
  
	bool SendToSetGuideManual(const string& ip, int port);
	bool SendToSetGuideAuto(const string& ip, int port);
	bool SendToSetGuideCenter(const string& ip, int port);
}
#endif