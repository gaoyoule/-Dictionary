#include "worddao.h"

WordDao::WordDao()
{

}

WordDao::~WordDao()
{

}

//查询单词的解释，通过引用，将解释返回
bool WordDao::searchWord(QString word, QString &explanation)
{
    QSqlQuery query;
    QString sql = QString("select explanation from word where word = '%1';").arg(word);
    if(!query.exec(sql))//执行数据库操作
    {
        qDebug()<<sql<<"语句执行失败!!";
        exit(-1);
    }
    if(query.next())//说明查找到单词
    {
        explanation = query.value(0).toString();
        return true;//说明查询到该单词
    }
    return false;//说明查询的单词不存在
}


