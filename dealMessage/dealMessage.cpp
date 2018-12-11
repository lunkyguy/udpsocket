#include "dealMessage.h"

#define g_ip	"127.0.0.1"
#define g_multi_ip "225.0.0.1"
#define g_port	8888

#define MAX_LEN	256
GetMessageThread::GetMessageThread()
{
}


GetMessageThread::~GetMessageThread()
{
}

// 启用线程
void GetMessageThread::run()
{
#ifdef WIN32
	WSADATA wsa;
	int ierr = WSAStartup(MAKEWORD(2, 2), &wsa);
	if (ierr != 0)
	{
		qDebug() << "WSAStartup faile";
	}
#endif // WIN32
	if ((m_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		qDebug() << "socket faile";
	}

	int iloop = 1;
	if (-1 == setsockopt(m_socket, IPPROTO_IP, IP_MULTICAST_LOOP, (const char*)&iloop, sizeof(iloop)))
	{
		qDebug() << "setsockopt faile";
	}

	sockaddr_in m_addr;
	m_addr.sin_family = AF_INET;
	m_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	m_addr.sin_port = htons(g_port);

	if (-1 == bind(m_socket, (const sockaddr*)&m_addr, sizeof(m_addr)))
	{
		qDebug() << "bind faile";
	}

	struct ip_mreq mreq;

	mreq.imr_multiaddr.s_addr = inet_addr(g_multi_ip);
	mreq.imr_interface.s_addr = inet_addr(g_ip);

	if (-1 == setsockopt(m_socket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (const char*)&mreq, sizeof(mreq)))
	{
		qDebug() << "setsockopt faile";
	}

	char buf[MAX_LEN] = { 0 };
	sockaddr_in clientAddr;
#ifdef WIN32
	int ifromLen = sizeof(sockaddr_in);
#else
	socklen_t  ifromLen = sizeof(sockaddr_in);
#endif // WIN32

	while (true)
	{
		int irecv = recvfrom(m_socket, buf, MAX_LEN, 0, (sockaddr*)&clientAddr, &ifromLen);
		emit updateRevSignal(buf);
		//if (irecv > 0)
		//{
		//	qDebug() << "recv = " << QString::fromLocal8Bit(buf);
		//}
	}
#ifdef WIN32
	closesocket(m_socket);
	WSACleanup();
#else
	close(m_socket);
#endif // WIN32
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
#ifdef WIN32
	WSADATA wsa;
	int ierr = WSAStartup(MAKEWORD(2, 2), &wsa);
	if (ierr != 0)
	{
		qDebug() << "WSAStartup faile";
	}
#endif // DEBUG
	m_socket = socket(AF_INET, SOCK_DGRAM, 0);
	if ((m_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		qDebug() << "socket faile";
	}

	int ioptv = 0;
	if (-1 == setsockopt(m_socket, SOL_SOCKET, SO_BROADCAST, (const char*)&ioptv, sizeof(ioptv)))
	{
		qDebug() << "setsockopt faile";
	}

	sockaddr_in m_addr;
	//ZeroMemory(&m_addr, sizeof(m_addr));
	m_addr.sin_family = AF_INET;
	m_addr.sin_addr.s_addr = inet_addr(g_multi_ip);
	m_addr.sin_port = htons(g_port);

	int tmp = 0;

	char buf[MAX_LEN];
	memset(buf, 0, MAX_LEN);
	while (true)
	{
		int isend = sendto(m_socket, buf, MAX_LEN, 0, (sockaddr*)&m_addr, sizeof(m_addr));
		tmp++;
		sprintf(buf, "%s%d%", "你好,socket", tmp);
		tmp = tmp % 100;
		emit updateSendSignal(buf);
		if (isend != MAX_LEN)
		{
			qDebug()<<"sendto faile";
		}
		QThread::msleep(30);
	}
#ifdef WIN32
	closesocket(m_socket);
	WSACleanup();
#else
	close(m_socket);
#endif // WIN32
}
