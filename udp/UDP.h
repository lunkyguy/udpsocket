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

//服务端接收消息
class UDPServer
{
private:
	#ifdef WIN32
		WSADATA wsaData;
		SOCKET serverSocket;
	#else
		int serverSocket; //
	#endif
	sockaddr_in serverSockAddr;
public:
	UDPServer();
	~UDPServer();
	void listen(const int port);
	void receive() const;
};

//客户端发送消息
class UDPClient
{
private:
#ifdef WIN32
	WSADATA wsaData;
	SOCKET clientSocket;
#else
	int clientSocket;
#endif
	sockaddr_in clientSockAddr;
public:
	UDPClient();
	~UDPClient();
	void setSockAddr(const int destPort);
	void sendData(const char *buf, const int len) const;
};
