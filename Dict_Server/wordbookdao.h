#ifndef WORDBOOKDAO_H
#define WORDBOOKDAO_H
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QDebug>

class WordBookDao
{
public:
    WordBookDao();
    ~WordBookDao();
    //将用户名 添加单词 添加单词的解释，保存到单词本中
    void insertIntoWordBook(QString username, QString word, QString explanation);
    //查询添加单词本的单词是否存在
    bool isExistAddWord(QString username, QString word);
    //将背诵单词用户的所有的解释放到列表里面返回
    QStringList getExplanationFromWordBook(QString username);
    //查询单词是否正确
    bool isCheckWordRight(QString username, QString word);
};

#endif // WORDBOOKDAO_H
