#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <QTextCodec>

#include "dealMessage/dealMessage.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
	Ui::MainWindow *ui;
	QTextCodec *codec;
private:
	QLabel *sendMsg, *revMsg;
	QLineEdit *sendText, *revText;
	SendMessageThread *sendMessageThread;
	GetMessageThread *getMessageThread;
private slots:
	void updateSendMsg(char *);
	void updateRecMsg(char *);
};
#endif // MAINWINDOW_H
