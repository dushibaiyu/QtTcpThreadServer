#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include <QtCore>
#include <QtNetwork>
#include "mytcpsocket.h"
#include "tcpsocketthread.h"
#include <QThread>

class MyTcpServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit MyTcpServer(QObject *parent = 0);
    ~MyTcpServer();
signals:
    void connectClient(int , QString , quint16 );//发送新用户连接信息
    void readData(int, QString, quint16, QByteArray);//发送获得用户发过来的数据
    void sockDisConnect(int , QString , quint16 );//断开连接的用户信息
    void sentData(QByteArray);
public slots:
    void setData(QByteArray data, int  handle);//想用户发送消息
    void readDataSlot(int handle, QString ip, quint16 prot, QByteArray data);//发送获得用户发过来的数据
//    void sockDisConnectSlot(int handle, QString ip, quint16 prot);//断开连接的用户信息
//    void socketConnectSlot(int handle, QString ip, quint16 prot);
protected:
    void incomingConnection(qintptr socketDescriptor);//覆盖已获取多线程
private:
    QMap<int,TcpSocketThread *> * tcpClient;
//    QMap<int,myTcpSocket *> * tcpClient;
};

#endif // MYTCPSERVER_H
