#include "doclientthread.h"

DoClientThread::DoClientThread(QWidget *parent) : QThread(parent)
{

}

DoClientThread::DoClientThread(QTcpSocket *tcpSocket):m_TcpScoket(tcpSocket),m_index(0)
{
    srand(time(NULL));//初始化随机种子
    //将tcp套接字进行信号与槽的绑定
    connect(m_TcpScoket, SIGNAL(readyRead()),this, SLOT(slotRecvClientData()));
}

DoClientThread::~DoClientThread()
{

}

//重写run方法
void DoClientThread::run()
{
    while(true)
    {
        QThread::msleep(1000);
    }
}
//处理注册请求的函数
void DoClientThread::doRegiterRequest(CJsonObject &json)
{
    DaoFactory *dao = DaoFactory::getInstance();
    string username, passwd;
    //解析出注册的用户名和密码
    json.Get("username",username);
    json.Get("passwd",passwd);
    json.Clear();//清空
    json.Add("type",1);//说明是对注册请求的结果反馈
    //拿用户名和密码去数据库与userInfo表做比对，判断注册是否成功，告诉客户端
    if(dao->m_UserInfoDao->isExistUserName(QString::fromStdString(username)))//说明用户存在，告诉服务器注册失败
        json.Add("registerResult", -1);//-1代表注册失败，用户名已经存在
    else//告诉服务器注册成功
    {   //QString temp = QString::fromStdString(username) 将 string ----> QString
        json.Add("registerResult", 0);//0代表注册成功
        dao->m_UserInfoDao->insertIntoUserInfo(QString::fromStdString(username), QString::fromStdString(passwd));//将用户名和密码保存到user_info表中
    }
    //将注册的结果回传给客户端
    QString data = QString(json.ToString().c_str());
    qDebug()<<"回传给客户端注册结果:"<<data;
    m_TcpScoket->write(data.toUtf8());
}

//处理登录请求的函数
void DoClientThread::doLoginRequest(CJsonObject &json)
{
    DaoFactory* dao = DaoFactory::getInstance();
    string username, passwd;
    //解析出登录的用户名和密码
    json.Get("username",username);
    json.Get("passwd",passwd);

    json.Clear();//将json对象中原来的键值对清除
    json.Add("type",2);//说明是对登录请求的结果反馈
    //拿登录的用户名和密码去数据中比对，是否正确,给客户端反馈
    if(dao->m_UserInfoDao->isLoginRight(QString::fromStdString(username), QString::fromStdString(passwd)))
    {
        json.Add("loginResult", 0);//用0来代表登录成功
        //将登录成功的这个用户名保存起来
        m_UserName = QString::fromStdString(username);
    }
    else if(dao->m_UserInfoDao->isExistUserName(QString::fromStdString(username)))
        json.Add("loginResult", -1);//用-1登录失败的原因是登录的密码错误
    else
        json.Add("loginResult", -2);//用-2登录失败的原因是登录的用户名不存在
    //将注册的结果回传给客户端
    QString data = QString(json.ToString().c_str());
    qDebug()<<"回传给客户端登录结果:"<<data;
    m_TcpScoket->write(data.toUtf8());
}

//处理查询单词请求的函数
void DoClientThread::doSearchWordRequest(CJsonObject &json)
{
    QString explanation;//用来保存单词的解释
    DaoFactory *dao = DaoFactory::getInstance();
    string word;//用来保存查询的单词
    //解析出客户端发送过来的单词
    json.Get("word",word);
    //去数据库中word单词表中去查询单词，将结果反馈给客户端
    json.Clear();//清空
    json.Add("type",3);//说明是对查询单词请求的结果反馈
    if(dao->m_WordDao->searchWord(QString::fromStdString(word),explanation))
    {
        json.Add("explanation", explanation.toStdString());
        //将用户名 查询时间  查询单词 保存到历史记录表
        QString t = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
        dao->m_HistoryInfoDao->insertIntoHistoryInfo(m_UserName, t, QString::fromStdString(word));
    }
    else
    {
        string s = QString("%1 not find!!").arg(QString::fromStdString(word)).toStdString();
        json.Add("explanation",s);
    }
    QString data = QString(json.ToString().c_str());
    qDebug()<<"回传给客户端查询单词结果:"<<data;
    m_TcpScoket->write(data.toUtf8());

}

//处理查询历史记录请求的函数
void DoClientThread::doHistoryInfoRequest(CJsonObject &json)
{
    DaoFactory *dao = DaoFactory::getInstance();
    QString s;
    //获取所有的历史信息
    QStringList list = dao->m_HistoryInfoDao->getHistoryInfo(m_UserName);
    for(int i = 0; i < list.size(); i++)
        s += list[i];
    json.Add("historyInfo", s.toStdString());
    QString data = QString(json.ToString().c_str());
    m_TcpScoket->write(data.toUtf8());
}

//处理添加单词本请求
void DoClientThread::doAddWordRequest(CJsonObject &json)
{
    DaoFactory *dao = DaoFactory::getInstance();
    string word;
    QString explanation;
    json.Get("word",word);//取出要添加到单词本的单词
    //先判断要添加到单词本的单词，在词库中是否存在
    if(dao->m_WordDao->searchWord(QString::fromStdString(word), explanation))//说明单词存在
    {
        if(dao->m_WordBookDao->isExistAddWord(m_UserName, QString::fromStdString(word)))
        {
            json.Add("addWordResult",-2); //用-2代表背诵单词本中已经添加该单词，不能重复添加
        }
        else
        {
            //将用户名 单词 解释添加到 单词本 wordBook表中
            dao->m_WordBookDao->insertIntoWordBook(m_UserName, QString::fromStdString(word), explanation);
            json.Add("addWordResult", 0);//用0代表添加单词成功
        }
    }
    else//添加单词本的单词不存在
    {
        json.Add("addWordResult",-1);//用-1代表添加单词失败
    }
    //将添加单词本的结果回传给客户端
    QString data = QString(json.ToString().c_str());
    m_TcpScoket->write(data.toUtf8());
}

//处理背诵单词请求函数
void DoClientThread::doMemorizeWordRequest(CJsonObject &json)
{
    int post;
    DaoFactory *dao = DaoFactory::getInstance();
    QStringList list = dao->m_WordBookDao->getExplanationFromWordBook(m_UserName);
    if(list.size() == 0)
    {
        json.Add("MemorizeWordResult", -1);//用-1来代表还没有添加单词到记录本中
    }
    else
    {
        do
        {
            post = rand() % list.size();
        }while(m_index == post);
        m_index = post;//记录位置下标，下一次生成，不能与该位置下标一样
        json.Add("MemorizeWordResult", 0);//用0来表达单词本中有添加的单词
        json.Add("explanation", list[post].toStdString());
    }
    //将需要背诵单词的解释发送给客户端
    QString data = QString(json.ToString().c_str());
    m_TcpScoket->write(data.toUtf8());
}

//处理校验单词的请求
void DoClientThread::doCheckWordRequest(CJsonObject &json)
{
    DaoFactory *dao = DaoFactory::getInstance();
    string word;
    json.Get("word",word);//取出客户端发送过来的单词
    json.Clear();
    json.Add("type",7);//对应的是校验单词的结果反馈
    if(dao->m_WordBookDao->isCheckWordRight(m_UserName, QString::fromStdString(word)))
        json.Add("checkResult", 0);//用0代表校验成功
    else
        json.Add("checkResult", -1);//用-1代表校验失败
    //将校验单词的结果发送给服务器
    QString data = QString(json.ToString().c_str());
    m_TcpScoket->write(data.toUtf8());
}


//接收客户端数据的槽函数
void DoClientThread::slotRecvClientData()
{
    int type;
    QByteArray data = m_TcpScoket->readAll();//读取客户端发送过来的所有数据
    qDebug()<<data;//将接收到的数据打印
    string s = data.toStdString();//将字节数组转换为string
    //通过cJsonObject对象对s进行解析
    CJsonObject json(s); //将接收到的数据传递给json对象,进行解析
    json.Get("type",type);//获取请求的类型
    switch(type) //判断客户端的请求类型
    {
    case 1://说明是注册请求
        doRegiterRequest(json);//处理注册请求的函数
        break;
    case 2://说明是登录请求
        doLoginRequest(json);//处理登录请求的函数
        break;
    case 3://说明是查询单词请求
        doSearchWordRequest(json);
        break;
    case 4://说明是查询历史记录请求
        doHistoryInfoRequest(json);
        break;
    case 5://说明是添加单词本请求
        doAddWordRequest(json);
        break;
    case 6://说明是背单词的请求
        doMemorizeWordRequest(json);
        break;
    case 7://说明是校验单词的请求
        doCheckWordRequest(json);
        break;
    }

}

