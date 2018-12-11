#pragma once
#include <qthread.h>
#include <qmutex.h>
#include<qdebug.h>
#ifdef WIN32
#include<WS2tcpip.h> //multicast
#include<WinSock2.h>
#pragma comment(lib,"ws2_32.lib")  
#else
#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>//close()
#include<netinet/in.h>//struct sockaddr_in
#include<arpa/inet.h>//inet_ntoa
#endif // WIN32

//接收报文
class GetMessageThread :
	public QThread
{
	Q_OBJECT
public:
	GetMessageThread();
	~GetMessageThread();
	// 启用线程
	void run();
private:
#ifdef WIN32
	SOCKET m_socket; //windows下套接字类型
	//bool broadcast = true;
#else
	int m_socket; //Linux下套接字类型
	//int broadcast = 1;
#endif
signals:
	void updateRevSignal(char*);
};

//发送报文
class SendMessageThread :
	public QThread
{
	Q_OBJECT
public:
	SendMessageThread();
	~SendMessageThread();
	// 启用线程
	void run();
private:
#ifdef WIN32
	SOCKET m_socket; //windows下套接字类型
	//bool broadcast = true;
#else
	int m_socket; //Linux下套接字类型
	//int broadcast = 1;	
#endif
signals:
	void updateSendSignal(char*);
};

