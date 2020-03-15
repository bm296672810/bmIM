#include <thread>
#include <chrono>
#include <assert.h>
#include <iostream>

#include "client.h"

Client::Client(const string& ip, int port)
{
    InitSocket(ip, port);
}
void Client::InitSocket(const string& ip, int port)
{
    WORD sockVersion = MAKEWORD(2, 2);
    WSADATA data;
    if (WSAStartup(sockVersion, &data) != 0)
    {
        return;
    }

    //m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    m_address = new Address(ip, port);
}
int Client::Send(const string& data)
{
    return ::send(m_socket, data.c_str(), (int)data.length(), 0);
}

int Client::Recv(string& data)
{
    int bufLen = 10, recvLen = 0;
    char* buf = new char[bufLen + 1];
    do {
        memset(buf, 0, bufLen + 1);
        recvLen = ::recv(m_socket, buf, bufLen + 1, 0);
        if(recvLen > 0)
            data += string(buf, recvLen);
    } while (recvLen >= bufLen);

    return (int)data.length();
}

int Client::socketTcpAlive(SOCKET socket)
{
    int ret = 0;

    int keep_alive = 1;
    ret = setsockopt(socket, SOL_SOCKET, SO_KEEPALIVE, (char*)&keep_alive, sizeof(keep_alive));

    if (ret == SOCKET_ERROR)
    {
        printf("setsockopt failed: %d \n", WSAGetLastError());
        return -1;
    }

    struct tcp_keepalive in_keep_alive = { 0 };
    unsigned long ul_in_len = sizeof(struct tcp_keepalive);
    struct tcp_keepalive out_keep_alive = { 0 };
    unsigned long ul_out_len = sizeof(struct tcp_keepalive);
    unsigned long ul_bytes_return = 0;

    in_keep_alive.onoff = 1;                    /*打开keepalive*/
    in_keep_alive.keepaliveinterval = 5000; /*发送keepalive心跳时间间隔-单位为毫秒*/
    in_keep_alive.keepalivetime = 1000;         /*多长时间没有报文开始发送keepalive心跳包-单位为毫秒*/

    ret = WSAIoctl(socket, SIO_KEEPALIVE_VALS, (LPVOID)&in_keep_alive, ul_in_len,
        (LPVOID)&out_keep_alive, ul_out_len, &ul_bytes_return, NULL, NULL);

    if (ret == SOCKET_ERROR)
    {
        printf("WSAIoctl failed: %d \n", WSAGetLastError());
        return -1;
    }

    return 0;
}
SOCKET Client::tcpConnectServer()
{
    connectStatus = 0;
    
    sockaddr_in* serverAddr = to_sockaddr(m_address);
    m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_socket == -1)
        return INVALID_SOCKET;

    connectStatus = ::connect(m_socket, (struct sockaddr*)serverAddr, sizeof(*serverAddr));
    if (connectStatus == SOCKET_ERROR)
    {
        int errNO = WSAGetLastError();
        printf("connect errorNo:%d !\n", errNO);
        closesocket(m_socket);

        m_bIsConnected = false;
    }
    else
        m_bIsConnected = true;

    return m_socket;
}

bool Client::Connect()
{
    if (m_bIsConnected)
        return true;

    if (tcpConnectServer() && connectStatus != -1)
    {
        socketTcpAlive(m_socket);
        m_bIsConnected = true;

        SetTimeOut(m_nReadTimeOut, m_nWriteTimeOut);
    }
    else
        m_bIsConnected = false;

    return m_bIsConnected;
}
bool Client::DisConnect()
{
    closesocket(m_socket);
    delete m_address;
    m_address = NULL;

    return true;
}
void Client::SetTimeOut(int readTimeOut, int writeTimeOut)
{
    m_nReadTimeOut = readTimeOut;
    m_nWriteTimeOut = writeTimeOut;
    setsockopt(m_socket, SOL_SOCKET, SO_SNDTIMEO, (char *)&m_nReadTimeOut, sizeof(int));
    setsockopt(m_socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&m_nWriteTimeOut, sizeof(int));
}

Client::~Client()
{
    DisConnect();
}
bool Client::ReadData(unsigned char *recvData, int readLength, int *resLength)
{
    int nRes;
    //assert(m_bIsConnected);
    if (!m_bIsConnected)
        return false;
    nRes = ::recv(m_socket, (char*)recvData, 2048, 0);

    if (nRes == SOCKET_ERROR)
    {
        closesocket(m_socket);
        m_bIsConnected = false;
        return false;
    }
    else {
        *resLength = nRes;
        return true;
    }
}

namespace udp
{
    const int BUF_SIZE = 64;

	bool refitySetGuideManualData(const string& ip, int port)
	{
		bool r = false;
		r = SendToSetGuideManual(ip, port);
		return r;
	}

	bool refitySetGuideAutoData(const string& ip, int port)
	{
		bool r = false;
		r = SendToSetGuideAuto(ip, port);
		return r;
	}

	bool refitySetGuideCenterData(const string& ip, int port)
	{
		bool r = false;
		r = SendToSetGuideCenter(ip, port);
		return r;
	}

    SOCKET _udpClientInit(const char* ip, int port, sockaddr** addr)
    {
        WSADATA     wsd;        //WSADATA变量
        SOCKET      sClient;    //客户端套接字
        SOCKADDR_IN servAddr;   //服务器地址

        servAddr.sin_family = AF_INET;
        if (ip)
            inet_pton(AF_INET, ip, (void*)&servAddr.sin_addr.S_un.S_addr);
        else
            servAddr.sin_addr.S_un.S_addr = INADDR_ANY;

        servAddr.sin_port = htons((short)port);
        int	nServAddlen = sizeof(servAddr);
        if (!(*addr))
            *addr = (sockaddr*)malloc(nServAddlen);

        memcpy_s(*addr, nServAddlen, &servAddr, nServAddlen);

        if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
        {
            cout << "WSAStartup failed!" << endl;
            return INVALID_SOCKET;
        }

        // 创建服务端套接字
        sClient = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        if (INVALID_SOCKET == sClient)
        {
            cout << "Socket failed !" << endl;
            WSACleanup();               //释放套接字资源
            return  INVALID_SOCKET;
        }
        else
        {
            cout << "Client UDP Socket init !" << endl;
        }

        return sClient;
    }
    void _udpSocketClose(SOCKET socket)
    {
        closesocket(socket);	//关闭服务端套接字
        //WSACleanup();
    }
    SOCKET _udpServerInit(const char* ip, int port, sockaddr** addr)
    {
        SOCKET sServer;
        sServer = _udpClientInit(ip, port, addr);
        int r = ::bind(sServer, (LPSOCKADDR)*addr, sizeof(SOCKADDR_IN));
        if (SOCKET_ERROR == r)
        {
            cout << "bind error:" << WSAGetLastError() << endl;
            _udpSocketClose(sServer);
            return INVALID_SOCKET;
        }

        return sServer;
    }

    int _udpSend(const char* ip, int port, const char* bufSend)
    {
        SOCKET			sClient;			//客户端套接字
        sockaddr* addr = NULL;
        sClient = _udpClientInit(ip, port, &addr);

        int retVal;                         //返回值
        char closeSymbol[5] = "0";          //结束通信的标志
		char cSendData[58] = { 0 };
		retVal = sendto(sClient, std::string(cSendData, 58).c_str(), (int)std::string(cSendData, 58).length(), 0, addr, sizeof(*addr));
        //retVal = sendto(sClient, bufSend, strlen(bufSend), 0, addr, sizeof(*addr));
        if (SOCKET_ERROR == retVal)
        {
            cout << "send failed!" << endl;
            return -1;
        }

        _udpSocketClose(sClient);
        return 0;
    }

    int _udpRecv(const char* ip, int port, char** result)
    {
        SOCKET      sServer;            //客户端套接字
        sockaddr* addr = NULL;
        char        bufRecv[BUF_SIZE] = { 0 };  //接收数据缓冲
        sServer = _udpServerInit(ip, port, &addr);
        int retVal = -1;
        SOCKADDR_IN clientAddr;
        int nAddrLen = sizeof(clientAddr);
        
        if (*result == NULL)
        {
            *result = new char[BUF_SIZE];
            memset(*result, 0, BUF_SIZE);
        }

        memset(bufRecv, 0, BUF_SIZE);

		int m_nReadTimeOut = 1000;
		int m_nWriteTimeOut = 1000;
		setsockopt(sServer, SOL_SOCKET, SO_SNDTIMEO, (char *)&m_nReadTimeOut, sizeof(int));
		setsockopt(sServer, SOL_SOCKET, SO_RCVTIMEO, (char *)&m_nWriteTimeOut, sizeof(int));

        retVal = recvfrom(sServer, bufRecv, BUF_SIZE, 0, (sockaddr*)&clientAddr, &nAddrLen);
        if (SOCKET_ERROR == retVal)
        {// 接收失败则关闭服务端客户端套接字
            cout << "Recv Failed!" << endl;
            _udpSocketClose(sServer);
            return -1;
        }

        if (retVal <= BUF_SIZE)
        {
            bufRecv[BUF_SIZE - 1] = '\0';			// 接收的最后一位设为\0，避免烫烫的问题
            memcpy_s(*result, retVal, bufRecv, retVal);
            char str[INET_ADDRSTRLEN];
            cout << "Client IP：" << inet_ntop(AF_INET, &clientAddr.sin_addr, str, sizeof(str)) << endl;
            cout << "Data recv from Client UDP Socket: " << bufRecv << endl;
        }

        _udpSocketClose(sServer);
        return retVal;
    }

    static bool s_bThreadEnd = false;
    void t_refityFunc(const string& currentIp, int currentPort, double* value, bool* isConnect)
    {
        s_bThreadEnd = false;
        char* result = NULL;
        int nRecvLen = _udpRecv(currentIp.c_str(), currentPort, &result);
        cout << "t_func _udpRecv return:" << nRecvLen << endl;

        if (nRecvLen == 58)
        {
            *isConnect = true;
            byte recvBuf[100] = { 0 };
            memcpy_s(recvBuf, nRecvLen, result, nRecvLen);

            int nInt[4] = { 0 };
            int nPos = 0;

            recvBuf[58] = '\0';

            nInt[0] = recvBuf[20] / 16;
            nInt[1] = recvBuf[20] % 16;
            nInt[2] = recvBuf[21] / 16;
            nInt[3] = recvBuf[21] % 16;

            if ((nInt[0] & 0x8) == 0x8)
            {
                nPos = (((nInt[0] * 16 + nInt[1]) << 8) + nInt[2] * 16 + nInt[3]) - 65536;
            }
            else {
                nPos = ((nInt[0] * 16 + nInt[1]) << 8) + nInt[2] * 16 + nInt[3];
            }
            *value = (int)nPos * 0.1;
        }
        else
            *isConnect = false;

        s_bThreadEnd = true;
    }
 //   double refitySendData(const string& ip, int port, const string& currentIp, int currentPort, bool* isConnect)
 //   {
 //       char data[58] = { 0 };
 //       
 //       double value = 0.0;

 //       //thread t(t_refityFunc, currentIp, currentPort, &value, isConnect);
 //       //t.detach();
	//	Sleep(50);
 //       int rv = _udpSend(ip.c_str(), port, string(data, 58).c_str());

 //       //while (!s_bThreadEnd);
 //       double r = value;
	//	
	//	return r;
	//}
    double refitySendData(const string& ip, int port, const string& currentIp, int currentPort, bool* isConnect)
    {
        double value = 0.0;

        char data[58] = { 0 };

        thread t(t_refityFunc, currentIp, currentPort, &value, isConnect);
        t.detach();
        Sleep(50);
        _udpSend(ip.c_str(), port, string(data, 58).c_str());

        while (!s_bThreadEnd);
        double r = value;

        return r;
    }
	bool SendToSetGuideManual(const string& ip, int port)
	{
		WORD socketVersion = MAKEWORD(2, 2);
		WSADATA wsaData;
		if (WSAStartup(socketVersion, &wsaData) != 0)
		{
			return false;
		}
		SOCKET sclient = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        if (sclient == INVALID_SOCKET)
            return false;

		struct sockaddr_in* addr = (struct sockaddr_in*) malloc(sizeof(struct sockaddr_in));
        if (addr)
        {
            addr->sin_family = AF_INET;

            inet_pton(addr->sin_family, ip.c_str(), &(addr->sin_addr.S_un.S_addr));
            addr->sin_port = htons((u_short)port);
        }
        else
            return false;

		int len = sizeof(*addr);
		char cdata[58] = { 0 };
		cdata[5] = 0x01;

		int rLen = sendto(sclient, string(cdata, 58).c_str(), (int)string(cdata, 58).length(), 0, (sockaddr*)addr, len);
		Sleep(100);
		cdata[5] = 0x09;
		rLen = sendto(sclient, string(cdata, 58).c_str(), (int)string(cdata, 58).length(), 0, (sockaddr*)addr, len);
		closesocket(sclient);
		WSACleanup();

		if (rLen == string(cdata, 58).length())
			return true;
		else
			return false;
	}

	bool SendToSetGuideAuto(const string& ip, int port)
	{
		WORD socketVersion = MAKEWORD(2, 2);
		WSADATA wsaData;
		if (WSAStartup(socketVersion, &wsaData) != 0)
		{
			return false;
		}
		SOCKET sclient = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        if (sclient == INVALID_SOCKET)
            return false;

		struct sockaddr_in* addr = (struct sockaddr_in*) malloc(sizeof(struct sockaddr_in));
        if (addr)
        {
            addr->sin_family = AF_INET;

            inet_pton(addr->sin_family, ip.c_str(), &(addr->sin_addr.S_un.S_addr));
            addr->sin_port = htons((u_short)port);
        }
        else
            return false;

		int len = sizeof(*addr);
		char cdata[58] = { 0 };
		cdata[5] = 0x04;

		int rLen = sendto(sclient, string(cdata, 58).c_str(), (int)string(cdata, 58).length(), 0, (sockaddr*)addr, len);
		Sleep(100);
		cdata[5] = 0x0C;
		rLen = sendto(sclient, string(cdata, 58).c_str(), (int)string(cdata, 58).length(), 0, (sockaddr*)addr, len);
		closesocket(sclient);
		WSACleanup();

		if (rLen == string(cdata, 58).length())
			return true;
		else
			return false;
	}

	bool SendToSetGuideCenter(const string& ip, int port)
	{
		WORD socketVersion = MAKEWORD(2, 2);
		WSADATA wsaData;
		if (WSAStartup(socketVersion, &wsaData) != 0)
		{
			return false;
		}
		SOCKET sclient = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        if (sclient == INVALID_SOCKET)
            return false;

		struct sockaddr_in* addr = (struct sockaddr_in*) malloc(sizeof(struct sockaddr_in));
        if (addr)
        {
            addr->sin_family = AF_INET;

            inet_pton(addr->sin_family, ip.c_str(), &(addr->sin_addr.S_un.S_addr));
            addr->sin_port = htons((u_short)port);
        }
        else
            return false;

		int len = sizeof(*addr);
		char cdata[58] = { 0 };
		cdata[5] = 0x02;

		int rLen = sendto(sclient, string(cdata, 58).c_str(), (int)string(cdata, 58).length(), 0, (sockaddr*)addr, len);
		Sleep(100);
		cdata[5] = 0x0A;
		rLen = sendto(sclient, string(cdata, 58).c_str(), (int)string(cdata, 58).length(), 0, (sockaddr*)addr, len);
		closesocket(sclient);
		WSACleanup();

		if (rLen == string(cdata, 58).length())
			return true;
		else
			return false;
	}
}