#include "wordbookdao.h"

WordBookDao::WordBookDao()
{
    QSqlQuery query;
    //定义数据库操作语句, 如果不存在，创建表
    QString sql = "create table if not exists word_book(username varchar(50), word varchar(50), explanation varchar(50));";
    if(!query.exec(sql))//执行数据库操作
    {
        qDebug()<<sql<<"语句执行失败!!";
        exit(-1);
    }
    qDebug()<<sql<<"语句执行成功!!";
}

WordBookDao::~WordBookDao()
{

}

 //将用户名 添加单词 添加单词的解释，保存到单词本中
void WordBookDao::insertIntoWordBook(QString username, QString word, QString explanation)
{
    QSqlQuery query;
    QString sql = QString("insert into word_book values('%1','%2','%3');").arg(username).arg(word).arg(explanation);
    if(!query.exec(sql))//执行数据库操作
    {
        qDebug()<<sql<<"语句执行失败!!";
        exit(-1);
    }
    qDebug()<<sql<<"语句执行成功!!";
}

//查询添加单词本的单词是否存在
bool WordBookDao::isExistAddWord(QString username, QString word)
{
    QSqlQuery query;
    QString sql = QString("select * from word_book where word = '%1' and username = '%2';").arg(word).arg(username);
    if(!query.exec(sql))//执行数据库操作
    {
        qDebug()<<sql<<"语句执行失败!!";
        exit(-1);
    }
    if(query.next())//说明添加单词本的单词已经存在
    {
        return true;
    }
    return false;//说明添加单词本的单词不存在
}

//将背诵单词用户的所有的解释放到列表里面返回
QStringList WordBookDao::getExplanationFromWordBook(QString username)
{
    QStringList list;
    QSqlQuery query;
    QString sql = QString("select explanation from word_book where username = '%1';").arg(username);
    if(!query.exec(sql))//执行数据库操作
    {
        qDebug()<<sql<<"语句执行失败!!";
        exit(-1);
    }
    while(query.next())
    {
        list.append(query.value(0).toString());
    }
    return list;
}

//查询单词是否正确
bool WordBookDao::isCheckWordRight(QString username, QString word)
{
    QSqlQuery query;
    QString sql = QString("select * from word_book where username = '%1' and word = '%2';").arg(username).arg(word);
    if(!query.exec(sql))//执行数据库操作
    {
        qDebug()<<sql<<"语句执行失败!!";
        exit(-1);
    }
    if(query.next())//说明单词本中存在该单词，正确
        return true;
    return false;//说明单词本中不存在该单词，不正确
}

