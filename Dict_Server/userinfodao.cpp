#include "userinfodao.h"

UserInfoDao::UserInfoDao()
{
    QSqlQuery query;
    //定义数据库操作语句, 如果不存在，创建表
    QString sql = "create table if not exists user_info(username varchar(50), passwd varchar(50));";
    if(!query.exec(sql))//执行数据库操作
    {
        qDebug()<<sql<<"语句执行失败!!";
        exit(-1);
    }
    qDebug()<<sql<<"语句执行成功!!";
}

UserInfoDao::~UserInfoDao()
{

}
//判断用户名是否存在 true 存在 false 不存在
bool UserInfoDao::isExistUserName(QString username)
{
    QSqlQuery query;
    QString sql = QString("select * from user_info where username = '%1';").arg(username);
    if(!query.exec(sql))//执行数据库操作
    {
        qDebug()<<sql<<"语句执行失败!!";
        exit(-1);
    }
    if(query.next())//如果查询结果有内容，返回true,说明用户名存在
        return true;
    return false;//说明用户名不存在
}

void UserInfoDao::insertIntoUserInfo(QString username, QString passwd)
{
    QSqlQuery query;
    QString sql = QString("insert into user_info values('%1','%2');").arg(username).arg(passwd);
    if(!query.exec(sql))//执行数据库操作
    {
        qDebug()<<sql<<"语句执行失败!!";
        exit(-1);
    }
    qDebug()<<sql<<"语句执行成功!!";
}

bool UserInfoDao::isLoginRight(QString username, QString passwd)
{
    QSqlQuery query;
    QString sql = QString("select * from user_info where username = '%1' and passwd = '%2';").arg(username).arg(passwd);
    if(!query.exec(sql))//执行数据库操作
    {
        qDebug()<<sql<<"语句执行失败!!";
        exit(-1);
    }
    if(query.next())//如果查询结果有内容，说明登录成功
        return true;
    return false;//说明登录失败
}

