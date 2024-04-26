#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    showServerIPandPort();//显示服务器IP地址和端口号
}

MainWindow::~MainWindow()
{
    delete ui;
}
//显示服务器本机的IP地址和端口号
void MainWindow::showServerIPandPort()
{
    QString allIPv4;//将所有的IPv4地址拼接成一个字符串
    //1.获取本地主机名称
    QString localHostName = QHostInfo::localHostName();
    //2.根据本地主机的名字，来获取IP信息
    QHostInfo hostInfo = QHostInfo::fromName(localHostName);
    //3.将所有的IP地址保存QList中
    QList<QHostAddress> list = hostInfo.addresses();
    for(int i = 0; i < list.size(); i++)
    {
        //只过滤出IPv4的地址
        if(list[i].protocol() == QAbstractSocket::IPv4Protocol)//如果是IPv4的IP地址
        {
            allIPv4 += list[i].toString();
            allIPv4 += "  ";
//            qDebug()<<list[i].toString();
        }
    }
    //将所有的IPv4地址，显示在lineEdit中
    ui->lineEditIP->setText(allIPv4);
    ui->lineEditPort->setText("8080");

    //给开始监听按钮，设置一个快捷键 回车
    ui->pushButtonListen->setShortcut(Qt::Key_Return);
}

//点击开始监听按钮
void MainWindow::on_pushButtonListen_clicked()
{
    ui->pushButtonListen->setEnabled(false);//点击之后，按钮处于灰色状态，不允许再次被点击
    //创建一个TCP服务器,将本机的端口号给服务器
    m_TcpServer = new TcpServer(ui->lineEditPort->text().toInt());//创建一个TCP服务器对象
    //对信号与槽进行绑定
    connect(m_TcpServer, SIGNAL(signalSendClientInfo(QString,QString)), this, SLOT(slotRecvClientInfo(QString,QString)));
}

//将连接服务器的这个客户单的IP地址和端口号显示出来
void MainWindow::slotRecvClientInfo(QString clientIP, QString clientPort)
{
     QString s = "客户端:"+clientIP+" "+clientPort+" 连接服务器成功!!";
     ui->listWidget->addItem(s);
}
