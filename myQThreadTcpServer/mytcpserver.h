#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include <QtCore>
#include <QtNetwork>
#include "mytcpsocket.h"


//继承QTCPSERVER以实现多线程TCPscoket的服务器。
//如果socket的信息处理直接处理的话，很多新建的信号和槽是用不到的
class MyTcpServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit MyTcpServer(QObject *parent = 0);
    ~MyTcpServer();
signals:
    void connectClient(const int , const QString & ,const quint16 );//发送新用户连接信息
    void readData(const int,const QString &, quint16, const QByteArray &);//发送获得用户发过来的数据
    void sockDisConnect(int ,QString ,quint16);//断开连接的用户信息
    void sentData(const QByteArray &,const int);//向scoket发送消息
    void sentDisConnect(int i); //断开特定连接，并释放资源，-1为断开所有。
public slots:
    void clear(); //断开所有连接，线程计数器请0
    void setData(const QByteArray & data, const int  handle);//想用户发送消息
    void readDataSlot(const int, const QString &, const quint16,const QByteArray &);//发送获得用户发过来的数据
    void sockDisConnectSlot(int handle,const QString & ip, quint16 prot, QThread *th);//断开连接的用户信息

protected:
    void incomingConnection(qintptr socketDescriptor);//覆盖已获取多线程
private:
    QMap<int,myTcpSocket *> * tcpClient;//管理连接的map

};

#endif // MYTCPSERVER_H
