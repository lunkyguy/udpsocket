#include "dealMessage.h"
#include<WINSOCK2.H>
#pragma comment(lib, "ws2_32.lib")

GetMessageThread::GetMessageThread()
{
}


GetMessageThread::~GetMessageThread()
{
}

// 启用线程
void GetMessageThread::run()
{
	//初始化
	WSADATA wsaData;
	SOCKET RecvSocket;
	sockaddr_in RecvAddr;//服务器地址
	int Port = 4000;//服务器监听地址
	char RecvBuf[1024];//发送数据的缓冲区
	int BufLen = 1024;//缓冲区大小
	sockaddr_in SenderAddr;
	int SenderAddrSize = sizeof(SenderAddr);
	//初始化Socket
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	//创建接收数据报的socket
	RecvSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	//将socket与制定端口和0.0.0.0绑定
	RecvAddr.sin_family = AF_INET;
	RecvAddr.sin_port = htons(Port);
	RecvAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	bind(RecvSocket, (SOCKADDR *)&RecvAddr, sizeof(RecvAddr));
	//调用Recvfrom函数在绑定的socket上接收数据
	printf("接收报文\n");
	recvfrom(RecvSocket, RecvBuf, BufLen, 0, (SOCKADDR *)&SenderAddr, &SenderAddrSize);
	//关闭socket，结束接收数据
	printf("停止接收，关闭socket\n");
	closesocket(RecvSocket);
	//释放资源，退出
	printf("退出\n");
	WSACleanup();
	return;
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
	//初始化
	WSADATA wsaData;
	SOCKET SendSocket;
	sockaddr_in RecvAddr;//服务器地址
	int Port = 4000;//服务器监听地址
	char SendBuf[1024];//发送数据的缓冲区
	int BufLen = 1024;//缓冲区大小
	//初始化Socket
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	//创建Socket对象
	SendSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	//设置服务器地址
	RecvAddr.sin_family = AF_INET;
	RecvAddr.sin_port = htons(Port);
	RecvAddr.sin_addr.s_addr = inet_addr("192.168.137.1");
	//向服务器发送数据报
	printf("发送报文\n");
	sendto(SendSocket, SendBuf, BufLen, 0, (SOCKADDR *)&RecvAddr, sizeof(RecvAddr));
	//发送完成，关闭Socket
	printf("发送完成，关闭Socket\n");
	closesocket(SendSocket);
	printf("退出\n");
	WSACleanup();
	return;
}
