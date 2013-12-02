#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include <QtCore>
#include <QtNetwork>
#include "mytcpsocket.h"
#include <QThread>

//继承QTCPSERVER以实现多线程TCPscoket的服务器。
class MyTcpServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit MyTcpServer(QObject *parent = 0);
    ~MyTcpServer();
signals:
    void connectClient(const int , const QString & ,const quint16 );//发送新用户连接信息
    void readData(const int,const QString &, quint16, const QByteArray &);//发送获得用户发过来的数据
    void sockDisConnect(const int ,const QString &,const quint16 );//断开连接的用户信息
    void sentData(const QByteArray &,const int);//向scoket发送消息
public slots:
    void setData(const QByteArray & data, const int  handle);//想用户发送消息
    void readDataSlot(const int, const QString &, const quint16,const QByteArray &);//发送获得用户发过来的数据
    void sockDisConnectSlot(int handle, QString ip, quint16 prot);//断开连接的用户信息

protected:
    void incomingConnection(qintptr socketDescriptor);//覆盖已获取多线程
private:
    QMap<int,myTcpSocket *> * tcpClient;
};

#endif // MYTCPSERVER_H
