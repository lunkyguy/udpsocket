#include "dealMessage.h"
#include<WINSOCK2.H>
#pragma comment(lib, "ws2_32.lib")

GetMessageThread::GetMessageThread()
{
}


GetMessageThread::~GetMessageThread()
{
}

// �����߳�
void GetMessageThread::run()
{
	//��ʼ��
	WSADATA wsaData;
	SOCKET RecvSocket;
	sockaddr_in RecvAddr;//��������ַ
	int Port = 4000;//������������ַ
	char RecvBuf[1024];//�������ݵĻ�����
	int BufLen = 1024;//��������С
	sockaddr_in SenderAddr;
	int SenderAddrSize = sizeof(SenderAddr);
	//��ʼ��Socket
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	//�����������ݱ���socket
	RecvSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	//��socket���ƶ��˿ں�0.0.0.0��
	RecvAddr.sin_family = AF_INET;
	RecvAddr.sin_port = htons(Port);
	RecvAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	bind(RecvSocket, (SOCKADDR *)&RecvAddr, sizeof(RecvAddr));
	//����Recvfrom�����ڰ󶨵�socket�Ͻ�������
	printf("���ձ���\n");
	recvfrom(RecvSocket, RecvBuf, BufLen, 0, (SOCKADDR *)&SenderAddr, &SenderAddrSize);
	//�ر�socket��������������
	printf("ֹͣ���գ��ر�socket\n");
	closesocket(RecvSocket);
	//�ͷ���Դ���˳�
	printf("�˳�\n");
	WSACleanup();
	return;
}

SendMessageThread::SendMessageThread()
{
}


SendMessageThread::~SendMessageThread()
{
}

// �����߳�
void SendMessageThread::run()
{
	//��ʼ��
	WSADATA wsaData;
	SOCKET SendSocket;
	sockaddr_in RecvAddr;//��������ַ
	int Port = 4000;//������������ַ
	char SendBuf[1024];//�������ݵĻ�����
	int BufLen = 1024;//��������С
	//��ʼ��Socket
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	//����Socket����
	SendSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	//���÷�������ַ
	RecvAddr.sin_family = AF_INET;
	RecvAddr.sin_port = htons(Port);
	RecvAddr.sin_addr.s_addr = inet_addr("192.168.137.1");
	//��������������ݱ�
	printf("���ͱ���\n");
	sendto(SendSocket, SendBuf, BufLen, 0, (SOCKADDR *)&RecvAddr, sizeof(RecvAddr));
	//������ɣ��ر�Socket
	printf("������ɣ��ر�Socket\n");
	closesocket(SendSocket);
	printf("�˳�\n");
	WSACleanup();
	return;
}
