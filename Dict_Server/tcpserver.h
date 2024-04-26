#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>
#include "doclientthread.h"

class TcpServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit TcpServer(QWidget *parent = 0);
    TcpServer(quint16 port);//带参构造函数
    ~TcpServer();

signals://信号
    //Tcp服务器，给MainWindow发送一个signalSendClientInfo信号，传递客户端的IP地址的端口号
    void signalSendClientInfo(QString clientIP, QString clientPort);

public slots:
    //重写 incomingConnection函数，当客户端连接服务器incomingConnection自动被调用
    void incomingConnection(qintptr handler);
private://成员变量
    QTcpSocket *m_TcpSocket;//tcp套接字对象指针
    DoClientThread *m_DoClientThread;//与客户端交互线程对象指针
};

#endif // TCPSERVER_H
