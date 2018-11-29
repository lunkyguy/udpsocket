#include "dealMessage.h"

#include<ws2tcpip.h> //multicast
#include <winsock2.h>  
#pragma comment(lib,"ws2_32.lib")  

#define g_ip	"127.0.0.1"
#define g_multi_ip "225.0.0.1"
#define g_port	8888

GetMessageThread::GetMessageThread()
{
}


GetMessageThread::~GetMessageThread()
{
}

// 启用线程
void GetMessageThread::run()
{
	WSADATA wsa;

	int ierr = WSAStartup(MAKEWORD(2, 2), &wsa);
	if (ierr != 0)
	{
		printf("WSAStartup faile\n");
	}

	SOCKET m_socket = socket(AF_INET, SOCK_DGRAM, 0);
	if (INVALID_SOCKET == m_socket)
	{
		printf("socket faile\n");
	}

	int iloop = 1;
	if (SOCKET_ERROR == setsockopt(m_socket, IPPROTO_IP, IP_MULTICAST_LOOP, (const char*)&iloop, sizeof(iloop)))
	{
		printf("setsockopt faile\n");
	}

	sockaddr_in m_addr;
	m_addr.sin_family = AF_INET;
	m_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	m_addr.sin_port = htons(g_port);

	if (SOCKET_ERROR == bind(m_socket, (const sockaddr*)&m_addr, sizeof(m_addr)))
	{
		printf("bind faile\n");
	}

	struct ip_mreq mreq;

	mreq.imr_multiaddr.S_un.S_addr = inet_addr(g_multi_ip);
	mreq.imr_interface.S_un.S_addr = inet_addr(g_ip);

	if (SOCKET_ERROR == setsockopt(m_socket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (const char*)&mreq, sizeof(mreq)))
	{
		printf("setsockopt faile\n");
	}

	char buf[MAX_PATH] = { 0 };
	int irecv = 0;
	SOCKADDR  clientAddr;
	int ifromLen = sizeof(SOCKADDR);

	while (true)
	{
		irecv = recvfrom(m_socket, buf, MAX_PATH, 0, &clientAddr, &ifromLen);
		if (irecv > 0)
		{
			printf("recv = %s\n", buf);
		}
	}
	closesocket(m_socket);
	WSACleanup();
}

SendMessageThread::SendMessageThread()
{
}


SendMessageThread::~SendMessageThread()
{
}

// 启用线程
void SendMessageThread::run()
{
	WSADATA wsa;

	int ierr = WSAStartup(MAKEWORD(2, 2), &wsa);
	if (ierr != 0)
	{
		printf("WSAStartup faile\n");
	}

	SOCKET m_socket = socket(AF_INET, SOCK_DGRAM, 0);
	if (INVALID_SOCKET == m_socket)
	{
		printf("socket faile\n");
	}

	int ioptv = 0;
	if (SOCKET_ERROR == setsockopt(m_socket, SOL_SOCKET, SO_BROADCAST, (const char*)&ioptv, sizeof(ioptv)))
	{
		printf("setsockopt faile\n");
	}

	sockaddr_in m_addr;

	ZeroMemory(&m_addr, sizeof(m_addr));

	m_addr.sin_family = AF_INET;
	m_addr.sin_addr.S_un.S_addr = inet_addr(g_multi_ip);
	m_addr.sin_port = htons(g_port);

	char buf[MAX_PATH] = { "你好,socket" };
	int isend = 0;

	while (true)
	{
		isend = sendto(m_socket, buf, MAX_PATH, 0, (const sockaddr*)&m_addr, sizeof(m_addr));
		if (isend != MAX_PATH)
		{
			printf("sendto faile\n");
		}
	}
	closesocket(m_socket);
	WSACleanup();
}
