#ifndef HISTORYINFODAO_H
#define HISTORYINFODAO_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QDebug>

class HistoryInfoDao
{
public:
    HistoryInfoDao();
    ~HistoryInfoDao();
    void insertIntoHistoryInfo(QString username, QString time, QString word);//将用户名 时间 单词插入表中
    QStringList getHistoryInfo(QString username);//获取历史记录表里面的所有结果
};

#endif // HISTORYINFODAO_H
