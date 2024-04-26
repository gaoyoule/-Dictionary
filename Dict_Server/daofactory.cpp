#include "daofactory.h"

DaoFactory* DaoFactory::instance = nullptr;

//构造函数中打开数据库
DaoFactory::DaoFactory()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("test.db");//设置数据库文件的名字
    if(!db.open())
    {
        qDebug()<<"数据库打开失败";
        return;
    }
    qDebug()<<"数据库打开成功";

    //成员变量初始化
    m_UserInfoDao = new UserInfoDao;
    m_WordDao = new WordDao;
    m_HistoryInfoDao = new HistoryInfoDao;
    m_WordBookDao = new WordBookDao;

}

DaoFactory::~DaoFactory()
{
    db.close();
}

DaoFactory *DaoFactory::getInstance()
{
    if(instance == nullptr)
        instance = new DaoFactory;
    return instance;
}

