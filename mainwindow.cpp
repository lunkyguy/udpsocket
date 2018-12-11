#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
	codec = QTextCodec::codecForName("gbk");

	setFixedSize(400, 200);

	sendMsg = new QLabel(this);
	revMsg = new QLabel(this);
	sendText = new QLineEdit(this);
	revText = new QLineEdit(this);

	sendMsg->setText(codec->toUnicode("发送消息"));
	sendMsg->setGeometry(30, 60, 100, 15);
	revMsg->setText(codec->toUnicode("接收消息"));
	revMsg->setGeometry(30, 100, 100, 15);

	sendText->setGeometry(150, 60, 150, 15);
	sendText->setReadOnly(true);
	revText->setGeometry(150, 100, 150, 15);
	revText->setReadOnly(revText);

	sendMessageThread = new SendMessageThread;
	getMessageThread = new GetMessageThread;
	sendMessageThread->start();
	getMessageThread->start();
	connect(sendMessageThread, SIGNAL(updateSendSignal(char*)), this, SLOT(updateSendMsg(char*)));
	connect(getMessageThread, SIGNAL(updateRevSignal(char*)), this, SLOT(updateRecMsg(char*)));
}

MainWindow::~MainWindow()
{
    delete ui;
	if (sendMsg!=nullptr)
	{
		delete sendMsg;
	}
	
	if (revMsg != nullptr)
	{
		delete revMsg;
	}
	if (sendText != nullptr)
	{
		delete sendText;
	}
	if (revText != nullptr)
	{
		delete revText;
	}

	if (sendMessageThread != nullptr)
	{
		delete sendMessageThread;
	}
	if (getMessageThread != nullptr)
	{
		delete getMessageThread;
	}
}

void MainWindow::updateSendMsg(char *msg)
{
	sendText->setText(codec->toUnicode(msg));
}

void MainWindow::updateRecMsg(char *msg)
{
	revText->setText(codec->toUnicode(msg));
}

