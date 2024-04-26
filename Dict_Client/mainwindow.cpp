#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),m_TcpSocket(new QTcpSocket)
{
    ui->setupUi(this);
    setLineEdit();

    m_SearchWord = new SearchWord;//创建一个查询界面的对象
    connect(this, SIGNAL(signalSendTcpSocket(QTcpSocket*)),m_SearchWord, SLOT(slotRecvTcpSocket(QTcpSocket*)));
    connect(this, SIGNAL(signalSendExplanation(QString)),m_SearchWord, SLOT(slotRecvExplanation(QString)));
    connect(this, SIGNAL(signalSendHistoryInfo(QString)),m_SearchWord,SLOT(slotRecvHistoryInfo(QString)));
    connect(this, SIGNAL(signalSendAddWordResult(QString,int)),m_SearchWord,SLOT(slotRecvAddWrodResult(QString,int)));
    connect(this, SIGNAL(signalSendMemorizeWordResult(QString,int)),m_SearchWord,SLOT(slotRecvMemorizeWordResult(QString,int)));
    connect(this, SIGNAL(signalSendCheckResult(int)),m_SearchWord, SLOT(slotRecvCheckResult(int)));

    //将tcp套接字对象与槽函数绑定
    connect(m_TcpSocket,SIGNAL(readyRead()),this,SLOT(slotRecvServerData()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

//设置LineEdit中的默认显示
void MainWindow::setLineEdit()
{
    ui->lineEditIP->setText("192.168.31.216");
    ui->lineEditPort->setText("8080");
    ui->lineEditUserName->setText("admin");
    ui->lineEditPasswd->setText("123");
    //给连接按钮，设置一个快捷键，与回车关联
    ui->pushButtonConnect->setShortcut(Qt::Key_Return);
}

//点击连接按钮执行的槽函数
void MainWindow::on_pushButtonConnect_clicked()
{
    //容错判断
    if(ui->lineEditIP->text().isEmpty() || ui->lineEditPort->text().isEmpty())
    {
        QMessageBox::warning(this, "友情提示","忘记输入服务器IP和Port!!");
        return;
    }
    ui->pushButtonConnect->setEnabled(false);//处于不允许再次被点击的状态
    //连接服务器
    m_TcpSocket->connectToHost(QHostAddress(ui->lineEditIP->text()), ui->lineEditPort->text().toInt());
}

//点击注册按钮执行的槽函数
void MainWindow::on_pushButtonRegister_clicked()
{
    //容错判断
    if(ui->lineEditUserName->text().isEmpty() || ui->lineEditPasswd->text().isEmpty())
    {
        QMessageBox::warning(this, "友情提示","忘记输入注册的用户名和密码!!");
        return;
    }
    //创建cJsonObject对象,添加键值对
    string username = ui->lineEditUserName->text().toStdString();
    string passwd = ui->lineEditPasswd->text().toStdString();
    CJsonObject json;
    json.Add("type",1);//请求类型为1
    json.Add("username", username);//用户名
    json.Add("passwd",passwd);//用户密码
    QString data = QString(json.ToString().c_str());//通过json对象，格式化发送的字符串，保存到data中
    qDebug()<<"发送的字符串是:" << data;
    m_TcpSocket->write(data.toUtf8());//将数据发送给服务器
}

//接收服务器端回传回来的请求结果
void MainWindow::slotRecvServerData()
{
    int type;
    QByteArray data = m_TcpSocket->readAll();//读取出所有的数据
    string s = data.toStdString();
    qDebug()<<data;
    CJsonObject json(s);//创建json对象
    json.Get("type",type); //得到回传回来的请求结果类型
    //对回传回来的请求结果进行解析
    if(type == 1)//说明回传回来的是注册请求的结果
    {
        int registerRet;
        json.Get("registerResult",registerRet);
        QMessageBox::warning(this,"注册提示", registerRet==0 ? "恭喜您，注册成功!!" : "抱歉，注册失败!!");
    }
    else if(type == 2) //得到回传回来的是登录请求的结果
    {
        int loginRet;
        json.Get("loginResult",loginRet);
        if(loginRet == 0)//登录成功
        {

            this->hide();//将当前的注册登录窗口隐藏

            emit signalSendTcpSocket(m_TcpSocket);//将套接字发送给 查询单词界面对象
            m_SearchWord->show();//将查询单词界面显示出来
            //显示登录成功之后的新界面
            //           QMessageBox::warning(this,"登录提示","恭喜您，登录成功!!!");
        }
        else if(loginRet == -1)//登录失败原因是 用户密码错误
            QMessageBox::warning(this,"登录提示","登录失败，输入的密码不正确!!");
        else if(loginRet == -2)//登录失败的原因是 用户名输入错误
            QMessageBox::warning(this, "登录提示","登录失败,输入的用户名不正确!!");
    }
    else if(type == 3)//得到回传回来的是查询单词请求的结果
    {
        string explanation;
        json.Get("explanation",explanation);//取出单词的解释
        //将单词的解释，发送给SearchWord界面
        emit signalSendExplanation(QString::fromStdString(explanation));
    }
    else if(type == 4)//得到回传回来的是获取历史信息请求的结果
    {
        string historyInfo;
        json.Get("historyInfo",historyInfo);
        //将历史记录，发送给SearchWord界面
        emit signalSendHistoryInfo(QString::fromStdString(historyInfo));
    }
    else if(type == 5)//得到回传回来的是添加单词本的请求结果
    {
        int addWordRet;
        string word;
        json.Get("addWordResult",addWordRet);
        json.Get("word",word);
        emit signalSendAddWordResult(QString::fromStdString(word), addWordRet);
    }
    else if(type == 6)//得到回传回来的是背诵单词的请求结果
    {
        int memorizeWordRet;
        string explanation;
        json.Get("memorizeWordResult", memorizeWordRet);
        json.Get("explanation",explanation);
        emit signalSendMemorizeWordResult(QString::fromStdString(explanation), memorizeWordRet);
    }
    else if(type == 7)//得到回传回来的是单词校验的结果
    {
        int checkRet;
        json.Get("checkResult",checkRet);
        emit signalSendCheckResult(checkRet);
    }
}

//点击登录按钮执行的槽函数
void MainWindow::on_pushButtonLogin_clicked()
{
    //容错判断
    if(ui->lineEditUserName->text().isEmpty() || ui->lineEditPasswd->text().isEmpty())
    {
        QMessageBox::warning(this, "友情提示","忘记输入登录的用户名和密码!!");
        return;
    }
    CJsonObject json;
    //创建cJsonObject对象,添加键值对
    string username = ui->lineEditUserName->text().toStdString();
    string passwd = ui->lineEditPasswd->text().toStdString();
    json.Add("type",2);//请求类型为2,代表登录请求
    json.Add("username", username);//用户名
    json.Add("passwd",passwd);//用户密码
    QString data = QString(json.ToString().c_str());//通过json对象，格式化发送的字符串，保存到data中
    qDebug()<<"发送的字符串是:" << data;
    m_TcpSocket->write(data.toUtf8());//将数据发送给服务器
}
