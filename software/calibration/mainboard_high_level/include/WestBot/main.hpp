#ifndef MAIN_HPP
#define MAIN_HPP



#include <QCoreApplication>
#include <QTime>
#include <QTimer>
#include <QProcess>
#include <QThread>
#include <QDir>
#include <QSettings>
#include <QString>
#include <QEventLoop>
#include <QDebug>
#include <QTcpServer>
#include <QTcpSocket>

#include <QSerialPort>
#include <signal.h>

#include <Rtos.h>

#include "../Common/Message.h"

#define SYS_FIFO_SIZE 1000000

class SerialToTcp : public QObject
{
	Q_OBJECT

public:
	SerialToTcp(const char* uart, u32 baudrate, u32 tcpPort);
	~SerialToTcp();

private:
	QSerialPort*	uart;

	QTcpServer*		server			= NULL;
	QTcpSocket*		socket			= NULL;
	QTimer			timer;

	void tcpDisconnect();
	enum {
		TCP_UNCONNECTED,
		TCP_IDLE,
	}tcpStatus = TCP_UNCONNECTED;

private slots:
	void task();
	void tcpNewConnection();
};


class ISystem : public QObject
{
    Q_OBJECT

public:
    ISystem();
    ~ISystem();

private:

//TCP
    QTcpServer*	server			= NULL;
    QTcpSocket*	socket			= NULL;
    Fifo		tcpTx;
    Fifo		tcpRx;
    QTimer		tcpTimer;
	void tcpDisconnect();
    enum {
        TCP_UNCONNECTED,
        TCP_IDLE,
    }tcpStatus = TCP_UNCONNECTED;

//Ports série
	u32				uartNb = 2;
	SerialToTcp*	uart[2];
	const char*		uartName[2] = {"ttyUSB0","ttyUSB1"};
	u32				uartBrg[2] = {1000000,1000000};
	//u32				uartBrg[2] = {1000000,115200};
	u32				uartPort[2] = {1000,1001};

private slots:
    void tcpTask();
	void tcpNewConnection();
    static int messageCmdPosition(ISystem *sys, MsgCmdPosition *msg);

};//ISystem


#endif // MAIN_HPP
