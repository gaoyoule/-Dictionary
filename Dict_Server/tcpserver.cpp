#include "tcpserver.h"

TcpServer::TcpServer(QWidget *parent) : QTcpServer(parent)
{

}

TcpServer::TcpServer(quint16 port)
{
    listen(QHostAddress::AnyIPv4, port);//监听所有的IPv4地址
    //端口号来在于lineEdit中输入的端口号
}

TcpServer::~TcpServer()
{

}
//重写 incomingConnection函数，当客户端连接服务器incomingConnection自动被调用
void TcpServer::incomingConnection(qintptr handler)
{
    m_TcpSocket = new QTcpSocket;//创建一个套接字对象
    m_TcpSocket->setSocketDescriptor(handler);//用本地的套接字描述符初始化QAbstractSocket套接字
    //获取连接的服务器的那个客户端的IP地址和端口号
    qDebug()<<m_TcpSocket->peerAddress().toString();
    qDebug()<<m_TcpSocket->peerPort();
    //将连接服务器的这个客户端的IP地址和端口号发送给MainWindow
    emit signalSendClientInfo(m_TcpSocket->peerAddress().toString(), QString("%1").arg(m_TcpSocket->peerPort()));
    m_DoClientThread = new DoClientThread(m_TcpSocket);//创建一个线程,将套接字传递给线程
    m_DoClientThread->start(); //启动线程
}

