#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	//QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF8"));
	//QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF8"));
	//QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF8"));
	MainWindow w;
	w.show();
	return a.exec();
}
