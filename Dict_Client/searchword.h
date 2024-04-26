#ifndef SEARCHWORD_H
#define SEARCHWORD_H

#include <QMainWindow>
#include <QMessageBox>
#include <QDebug>
#include "CJsonObject.hpp"
#include <string>
#include <QTcpSocket>

using namespace neb;
using namespace std;


namespace Ui {
class SearchWord;
}

class SearchWord : public QMainWindow
{
    Q_OBJECT

public:
    explicit SearchWord(QWidget *parent = 0);
    ~SearchWord();

private slots:
    void on_pushButtonSearchWord_clicked();
    void slotRecvTcpSocket(QTcpSocket* tcpSocket);//接收MainWindow发送过来的tcp套接字
    void slotRecvExplanation(QString explanation);//接收MainWindow发送过来的解释
    void slotRecvHistoryInfo(QString historyInfo);//接收MainWindow发送过来的历史记录结果
    void slotRecvAddWrodResult(QString word, int addWordRet);//接收MainWindow发送过来的添加单词本的结果
    void slotRecvMemorizeWordResult(QString explanation, int memorizeWordRet);//接收MainWindow发送过来的背诵单词请求结果
    void slotRecvCheckResult(int checkRet);//接收MainWindow发送过来的单词校验的结果

    void on_pushButtonHistory_clicked();

    void on_pushButtonAddWord_clicked();

    void on_pushButtonMemoryWord_clicked();

    void on_pushButtonCheckWord_clicked();

private:
    Ui::SearchWord *ui;
    QTcpSocket *m_TcpSocket; //Tcp套接字
};

#endif // SEARCHWORD_H
