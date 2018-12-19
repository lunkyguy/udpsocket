#include"UDP.h"
#include <stdio.h>
#define MCAST_ADDR  "224.100.100.100"

//接收报文
UDPServer::UDPServer()
{
	//socket初始化
	#ifdef WIN32
		WSAStartup(MAKEWORD(2, 2), &wsaData);
	#endif // WIN32
	if((serverSocket = socket(AF_INET, SOCK_DGRAM,IPPROTO_UDP))<0)
	{
		perror("socket()");
		return;
	}
}

void UDPServer::listen(const int port)
{
	memset(&serverSockAddr, 0, sizeof(sockaddr_in));
	serverSockAddr.sin_family = AF_INET;
	serverSockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serverSockAddr.sin_port = htons(port);

	if (bind(serverSocket, (SOCKADDR*)&serverSockAddr, sizeof(serverSockAddr))<0)
	{
		perror("bind()");
		return;
	}
	//消息回送控制
	int loop = 1;
	if (setsockopt(serverSocket, IPPROTO_IP, IP_MULTICAST_LOOP, (const char*)&loop, sizeof(loop)) < 0)
	{
		perror("setsockopt():IP_MULTICAST_LOOP");
		return;
	}
	//添加组播
	struct ip_mreq mreq;
	mreq.imr_multiaddr.s_addr = inet_addr(MCAST_ADDR);
	mreq.imr_interface.s_addr = htonl(INADDR_ANY);
	if (setsockopt(serverSocket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (const char*)&mreq, sizeof(mreq)) < 0)
	{
		perror("setsockopt():IP_ADD_MEMBERSHIP");
		return;
	}
}

void UDPServer::receive() const
{
	char buf[1024];
	memset(buf, 0, sizeof(buf));
	#ifdef WIN32
		int serverSockAddrSize = sizeof(sockaddr_in);
	#else
		socklen_t  serverSockAddrSize = sizeof(sockaddr_in);
	#endif // WIN32
	if (recvfrom(serverSocket, buf, sizeof(buf), 0, (SOCKADDR *)&serverSockAddr, &serverSockAddrSize) < 0)
	{
		perror("recvfrom()");
		return;
	}
	printf("%s\n", buf);
}

UDPServer::~UDPServer()
{
	#ifdef WIN32
		closesocket(serverSocket);
		WSACleanup();
	#else
		close(serverSocket);
	#endif // WIN32
}

//发送报文
UDPClient::UDPClient()
{
	#ifdef WIN32
		WSAStartup(MAKEWORD(2, 2), &wsaData);
	#endif // WIN32
	if ((clientSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))<0)
	{
		perror("socket()");
		return;
	}
}

void UDPClient::setSockAddr(const int Port)
{
	memset(&clientSockAddr, 0, sizeof(sockaddr_in));
	clientSockAddr.sin_family = AF_INET;
	clientSockAddr.sin_addr.s_addr = inet_addr(MCAST_ADDR);
	clientSockAddr.sin_port = htons(Port);

}

void UDPClient::sendData(const char *buf, const int len) const
{
	if (sendto(clientSocket, buf, len, 0, (sockaddr*)&clientSockAddr, sizeof(clientSockAddr)) < 0)
	{
		perror("sendto()");
		return;
	}
}

UDPClient::~UDPClient()
{
#ifdef WIN32
	closesocket(clientSocket);
	WSACleanup();
#else
	close(clientSocket);
#endif // WIN32
}
