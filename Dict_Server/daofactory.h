#ifndef DAOFACTORY_H
#define DAOFACTORY_H
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QDebug>
#include "userinfodao.h"
#include "worddao.h"
#include "historyinfodao.h"
#include "wordbookdao.h"

//单例模式创建DaoFactory
class DaoFactory
{
private:
    static DaoFactory* instance;
    DaoFactory();
public: //成员函数
    ~DaoFactory();
    static DaoFactory* getInstance();

public: //成员变量
    QSqlDatabase db;//数据库操作句柄
    UserInfoDao *m_UserInfoDao;//操作用户信息表的对象指针
    WordDao *m_WordDao;//操作单词表的对象指针
    HistoryInfoDao *m_HistoryInfoDao;//操作历史记录表的对象指针
    WordBookDao *m_WordBookDao;//操作背诵单词记录表的对象指针
};

#endif // DAOFACTORY_H
