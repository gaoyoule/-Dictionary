#include "historyinfodao.h"

HistoryInfoDao::HistoryInfoDao()
{
    QSqlQuery query;
    //定义数据库操作语句, 如果不存在，创建表
    QString sql = "create table if not exists history_info(username varchar(50), time varchar(50), word varchar(50));";
    if(!query.exec(sql))//执行数据库操作
    {
        qDebug()<<sql<<"语句执行失败!!";
        exit(-1);
    }
    qDebug()<<sql<<"语句执行成功!!";
}

HistoryInfoDao::~HistoryInfoDao()
{

}
//将用户名 时间 单词插入表中
void HistoryInfoDao::insertIntoHistoryInfo(QString username, QString time, QString word)
{
    QSqlQuery query;
    QString sql = QString("insert into history_info values('%1','%2','%3');").arg(username).arg(time).arg(word);
    if(!query.exec(sql))//执行数据库操作
    {
        qDebug()<<sql<<"语句执行失败!!";
        exit(-1);
    }
    qDebug()<<sql<<"语句执行成功!!";
}

//获取历史记录表里面的所有结果
QStringList HistoryInfoDao::getHistoryInfo(QString username)
{
    QSqlQuery query;
    QStringList list;
    QString sql = QString("select * from history_info where username = '%1';").arg(username);
    if(!query.exec(sql))//执行数据库操作
    {
        qDebug()<<sql<<"语句执行失败!!";
        exit(-1);
    }
    qDebug()<<sql<<"语句执行成功!!";
    while(query.next())
    {
        //将用户名 时间 单词 格式化一行字符串
        QString s = QString("%1 | %2 | %3\n").arg(query.value(0).toString()).arg(query.value(1).toString()).arg(query.value(2).toString());
        list.append(s);
    }
    return list;
}

