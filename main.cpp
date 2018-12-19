#include"udp/UDP.h"
#include<thread>

const char buf[1024] = "This is a udp test message";

void sendMsg(UDPClient* client)
{
	while (1)
	{
		client->sendData(buf, sizeof(buf));
	}
}

void receiveMsg(UDPServer* server)
{
	while (1)
	{
		server->receive();
	}
}

int main()
{
	UDPServer* server =new UDPServer;
	UDPClient* client = new UDPClient;
	server->listen(4000);
	client->setSockAddr(4000);
	client->sendData(buf, sizeof(buf));
	std::thread t1(receiveMsg, server);
	std::thread t2(sendMsg, client);
	t1.join();
	t2.join();

	delete server;
	delete client;
}