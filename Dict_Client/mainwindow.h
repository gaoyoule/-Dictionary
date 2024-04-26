#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QHostAddress>
#include <QMessageBox>
#include "CJsonObject.hpp"
#include <string>
#include <QDebug>
#include "searchword.h"

using namespace neb;
using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setLineEdit();//设置LineEdit中的默认显示

private slots:
    void on_pushButtonConnect_clicked();

    void on_pushButtonRegister_clicked();
    void slotRecvServerData();//接收服务器发送过来的数据
    void on_pushButtonLogin_clicked();

signals:
    void signalSendTcpSocket(QTcpSocket* tcpSocket);//将tcp套接字发送给SearchWord对象
    void signalSendExplanation(QString explanation);//将查询的单词解释发送给SearchWord对象
    void signalSendHistoryInfo(QString historyInfo);//将查询历史记录的结果发送给SearchWord对象
    void signalSendAddWordResult(QString word, int addWordRet);//将添加单词本的结果发送给SearchWord对象
    void signalSendMemorizeWordResult(QString explanation, int memorizeWordRet);//将背诵单词请求的结果发送给SearchWord对象
    void signalSendCheckResult(int checkRet);//将单词校验的结果发送给SearchWord对象

private:
    Ui::MainWindow *ui;
    QTcpSocket *m_TcpSocket;//定义tcp套接字对象指针
    SearchWord *m_SearchWord;//定义查询单词界面对象指针
};

#endif // MAINWINDOW_H
