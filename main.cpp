#include <iostream>
#include <cstring>
#include <thread>

#ifdef _MSC_VER
#include <winsock2.h>
#include <Ws2tcpip.h>
#pragma comment(lib,"ws2_32.lib")  
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SOCKET int
#define closesocket close

#endif 

#define MCAST_ADDR  "224.100.100.100"
#define DEFAULT_PORT 27015
#define DEFAULT_BUFLEN 512

int main()
{
	auto udp_client_fun = []()
	{
		//1.创建套接字
#ifdef _MSC_VER
		WORD wVersionRequested = MAKEWORD(2, 2);
		WSADATA wsaData;
		if (WSAStartup(wVersionRequested, &wsaData) != 0)
		{
			std::cout << "WSAStartup error" << std::endl;
			return -1;
		}
#endif
		//2.创建SOCKET
		SOCKET client_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
		if (-1 == client_socket)
		{
			std::cout << "create error" << std::endl;
			return -1;
		}

		//3.设置IP和端口号
		sockaddr_in client_sockAddr;
		memset(&client_sockAddr, 0, sizeof(sockaddr_in));
		client_sockAddr.sin_family = AF_INET;
		inet_pton(AF_INET, MCAST_ADDR, &client_sockAddr.sin_addr.s_addr);
		client_sockAddr.sin_port = htons(DEFAULT_PORT);

		while (true)
		{
		//4.发送报文
			const char buf[DEFAULT_BUFLEN] = "This is a udp test message";
			if (sendto(client_socket, buf, DEFAULT_BUFLEN, 0, (const sockaddr*)&client_sockAddr, sizeof(sockaddr_in)) > 0)
			{
				std::cout << "send message" << std::endl;
				//return -1;
			}
		}
		closesocket(client_socket);
	};
	auto udp_server_fun = []()
	{
		//1.创建套接字
#ifdef _MSC_VER
		WORD wVersionRequested = MAKEWORD(2, 2);
		WSADATA wsaData;	
		if (WSAStartup(wVersionRequested, &wsaData) != 0)
		{
			std::cout << "WSAStartup error" << std::endl;
			return -1;
		}
#endif
		//2.创建SOCKET
		SOCKET server_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
		if (-1 == server_socket)
		{
			std::cout << "create error" << std::endl;
			return -1;
		}

		//3.设置IP和端口号
		sockaddr_in server_sockAddr;
		memset(&server_sockAddr, 0, sizeof(sockaddr_in));
		server_sockAddr.sin_family = AF_INET;
		server_sockAddr.sin_addr.s_addr = INADDR_ANY;
		server_sockAddr.sin_port = htons(DEFAULT_PORT);

		//4.添加组播
		struct ip_mreq mreq;
		inet_pton(AF_INET, MCAST_ADDR, &mreq.imr_multiaddr.s_addr);
		mreq.imr_interface.s_addr = htonl(INADDR_ANY);
		if (setsockopt(server_socket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (const char*)&mreq, sizeof(mreq)) < 0)
		{
			std::cout << "setsockopt():IP_ADD_MEMBERSHIP" << std::endl;
			closesocket(server_socket);
			return  -1;
		}
		
		//5.消息回送控制
		int loop = 1; //参数loop设置为0禁止回送，设置为1允许回送
		if (setsockopt(server_socket, IPPROTO_IP, IP_MULTICAST_LOOP, (const char*)&loop, sizeof(loop)) < 0)
		{
			std::cout << "setsockopt:IP_MULTICAST_LOOP error" << std::endl;
			closesocket(server_socket);
			return -1;
		}

		//6.端口复用
		int bOptval = 1;//参数loop设置为0禁止复用，设置为1允许复用
		if (setsockopt(server_socket, IPPROTO_IP, SO_REUSEADDR, (const char*)&bOptval, sizeof(bOptval)) < 0)
		{
			std::cout << "setsockopt:SO_REUSEADDR error" << std::endl;
			closesocket(server_socket);
			return  -1;
		}

		//7.绑定端口
		if (bind(server_socket, (const sockaddr*)&server_sockAddr, sizeof(sockaddr_in)) < 0)
		{
			std::cout << "bind error" << std::endl;
			closesocket(server_socket);
			return -1;
		}

		//8.接收报文
		while (true)
		{
			char buf[DEFAULT_BUFLEN];
			memset(buf, 0, DEFAULT_BUFLEN);
			socklen_t serverSockAddrSize = sizeof(sockaddr_in);
			if (recvfrom(server_socket, buf, sizeof(buf), 0, (sockaddr*)&server_sockAddr, &serverSockAddrSize) < 0)
			{
				std::cout << "recvfrom error" << std::endl;
				closesocket(server_socket);
				return -1;
			}	
			std::cout << buf << std::endl;
		}
		closesocket(server_socket);
	};

	std::thread t_udp_client = std::thread(udp_client_fun);
	std::thread t_udp_server = std::thread(udp_server_fun);

	if (t_udp_client.joinable()) t_udp_client.join();
	if (t_udp_server.joinable()) t_udp_server.join();
}