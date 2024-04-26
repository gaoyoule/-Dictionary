#include "searchword.h"
#include "ui_searchword.h"

SearchWord::SearchWord(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SearchWord)
{
    ui->setupUi(this);
}

SearchWord::~SearchWord()
{
    delete ui;
}

//点击查询单词按钮
void SearchWord::on_pushButtonSearchWord_clicked()
{
    //容错判断
    if(ui->lineEdit->text().isEmpty())//如果没有输入单词
    {
        QMessageBox::warning(this, "查询提示","忘记输入查询的单词!!");
        return;
    }
    CJsonObject json;
    //创建cJsonObject对象,添加键值对
    string word = ui->lineEdit->text().toStdString();
    json.Add("type",3);//请求类型为3,代表查询单词请求
    json.Add("word", word);//查询的单词

    QString data = QString(json.ToString().c_str());//通过json对象，格式化发送的字符串，保存到data中
    qDebug()<<"发送的字符串是:" << data;
    m_TcpSocket->write(data.toUtf8());//将数据发送给服务器
}

//接收Maindow发送过来的tcp套接字
void SearchWord::slotRecvTcpSocket(QTcpSocket *tcpSocket)
{
    m_TcpSocket = tcpSocket;
}
//接收MainWindow发送过来的解释
void SearchWord::slotRecvExplanation(QString explanation)
{
    ui->textBrowser->clear();//将原来的内容请求
    ui->textBrowser->setText(explanation);//显示单词的解释
}

//接收MainWindow发送过来的历史记录结果
void SearchWord::slotRecvHistoryInfo(QString historyInfo)
{
    ui->textBrowser->clear();
    ui->textBrowser->setText(historyInfo);
}

//接收MainWindow发送过来的添加单词本的结果
void SearchWord::slotRecvAddWrodResult(QString word, int addWordRet)
{
    if(addWordRet == -2)
        QMessageBox::warning(this,"添加单词提示",word+"在单词本中已经存在，不能重复添加!!!");
    else
        QMessageBox::warning(this, "添加单词提示", addWordRet == 0 ? word+"添加成功!!!" : word+"添加不存在!!!");
}

//接收MainWindow发送过来的背诵单词请求结果
void SearchWord::slotRecvMemorizeWordResult(QString explanation, int memorizeWordRet)
{
    if(memorizeWordRet == -1)
    {
        QMessageBox::warning(this, "背诵单词提示","抱歉，忘记添加单词到记录本中!!");
        return;
    }
    ui->textBrowser->clear();
    ui->textBrowser->setText(explanation);//将解释显示
}

//接收MainWindow发送过来的单词校验的结果
void SearchWord::slotRecvCheckResult(int checkRet)
{
    QString word = ui->lineEdit->text();
    QMessageBox::warning(this,"单词校验提示", checkRet == 0 ? word+"书写正确!!!" : word+"书写错误!!!");
}

//点击查询历史记录执行的槽函数
void SearchWord::on_pushButtonHistory_clicked()
{
    CJsonObject json;
    //创建cJsonObject对象,添加键值对
    json.Add("type",4);//请求类型为4,代表查询历史记录
    QString data = QString(json.ToString().c_str());//通过json对象，格式化发送的字符串，保存到data中
    qDebug()<<"发送的字符串是:" << data;
    m_TcpSocket->write(data.toUtf8());//将数据发送给服务器
}

//点击添加单词本按钮
void SearchWord::on_pushButtonAddWord_clicked()
{
    //容错判断
    if(ui->lineEdit->text().isEmpty())
    {
        QMessageBox::warning(this,"友情提示","忘记输入添加的单词!!");
        return;
    }
    CJsonObject json;
    //创建cJsonObject对象,添加键值对
    json.Add("type",5);//请求类型为5,代表添加单词本
    json.Add("word",ui->lineEdit->text().toStdString());//单词
    QString data = QString(json.ToString().c_str());//通过json对象，格式化发送的字符串，保存到data中
    qDebug()<<"发送的字符串是:" << data;
    m_TcpSocket->write(data.toUtf8());//将数据发送给服务器
}

//点击背单词按钮
void SearchWord::on_pushButtonMemoryWord_clicked()
{
    ui->textBrowser->clear();
    ui->lineEdit->clear();
    //这个请求的本质，是让服务器随机回传一个单词的解释，用来背单词
    CJsonObject json;
    //创建cJsonObject对象,添加键值对
    json.Add("type",6);//请求类型为6,背诵单词
    QString data = QString(json.ToString().c_str());//通过json对象，格式化发送的字符串，保存到data中
    qDebug()<<"发送的字符串是:" << data;
    m_TcpSocket->write(data.toUtf8());//将数据发送给服务器
}

//点击校验按钮执行的槽函数
void SearchWord::on_pushButtonCheckWord_clicked()
{
    //容错判断
    if(ui->lineEdit->text().isEmpty())
    {
        QMessageBox::warning(this,"友情提示","忘记输入背诵的单词!!");
        return;
    }
    CJsonObject json;
    //创建cJsonObject对象,添加键值对
    json.Add("type",7);//请求类型为7,校验单词
    json.Add("word",ui->lineEdit->text().toStdString());
    QString data = QString(json.ToString().c_str());//通过json对象，格式化发送的字符串，保存到data中
    qDebug()<<"发送的字符串是:" << data;
    m_TcpSocket->write(data.toUtf8());//将数据发送给服务器
}
