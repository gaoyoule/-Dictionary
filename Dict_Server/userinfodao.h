#ifndef USERINFODAO_H
#define USERINFODAO_H
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QDebug>

class UserInfoDao
{
public:
    UserInfoDao();
    ~UserInfoDao();
    //判断用户名是否存在 true 存在 false 不存在
    bool isExistUserName(QString username);
    //将用户名和密码插入到表中
    void insertIntoUserInfo(QString username, QString passwd);
    //判断用户名和密码是否正确 true 正确， false 不正确
    bool isLoginRight(QString username, QString passwd);
};

#endif // USERINFODAO_H
