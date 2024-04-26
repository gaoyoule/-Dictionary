#ifndef WORDDAO_H
#define WORDDAO_H
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QDebug>

class WordDao
{
public:
    WordDao();
    ~WordDao();
    bool searchWord(QString word, QString &explanation);//查询单词的解释，通过引用，将解释返回
};

#endif // WORDDAO_H
