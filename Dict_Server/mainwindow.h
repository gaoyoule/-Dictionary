#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QHostInfo>
#include <QDebug>
#include <QAbstractSocket>
#include "tcpserver.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void showServerIPandPort(); //显示服务器的IP和端口号

private slots:
    void on_pushButtonListen_clicked();
    void slotRecvClientInfo(QString clientIP, QString clientPort);

private:
    Ui::MainWindow *ui;
    TcpServer *m_TcpServer;//tcp服务器指针
};

#endif // MAINWINDOW_H
