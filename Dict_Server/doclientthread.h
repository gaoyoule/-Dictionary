#ifndef DOCLIENTTHREAD_H
#define DOCLIENTTHREAD_H

#include <QWidget>
#include <QThread>
#include <QTcpSocket>
#include <QDebug>
#include <string>
#include "CJsonObject.hpp"
#include "daofactory.h"
#include <QDateTime>
#include <ctime>

using namespace std;
using namespace neb;

class DoClientThread : public QThread
{
    Q_OBJECT
public:
    explicit DoClientThread(QWidget *parent = 0);
    DoClientThread(QTcpSocket* tcpSocket);//添加有参构造函数，将套接字对象传递给线程
    ~DoClientThread();
    void run();//重写run方法，让线程一直活着
    void doRegiterRequest(CJsonObject &json);//处理注册请求的函数
    void doLoginRequest(CJsonObject &json);//处理登录请求的函数
    void doSearchWordRequest(CJsonObject &json);//处理查询单词请求的函数
    void doHistoryInfoRequest(CJsonObject &json);//处理查询历史记录请求的函数
    void doAddWordRequest(CJsonObject &json);//处理添加单词本请求
    void doMemorizeWordRequest(CJsonObject &json);//处理背诵单词请求函数
    void doCheckWordRequest(CJsonObject &json);//处理校验单词的请求

signals:

public slots: //槽函数
    void slotRecvClientData();

private:
    QTcpSocket *m_TcpScoket;
    QString m_UserName;//用来保存登录成功的用户名
    int m_index;//用来保存上一次生成的随机数
};

#endif // DOCLIENTTHREAD_H
