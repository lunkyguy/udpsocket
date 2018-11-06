#include <QtCore/QCoreApplication>
#include "dealMessage.h"
#pragma warning(disable:4996)

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
	
	GetMessageThread getMessage;
	SendMessageThread sendMessage;
	getMessage.start();
	sendMessage.start();
	return a.exec();
}
