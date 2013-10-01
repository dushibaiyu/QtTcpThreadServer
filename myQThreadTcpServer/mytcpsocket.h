#ifndef MYTCPSOCKET_H
#define MYTCPSOCKET_H

#include <QtNetwork>
#include <QtCore>
#include <QThread>
//继承QTcpSocket以处理接收到的数据。
class myTcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit myTcpSocket(qintptr socketDescriptor,QObject *parent = 0);

signals:
    void readData(int, QString, quint16, QByteArray);//发送获得用户发过来的数据
    void sockDisConnect(int , QString , quint16 );//断开连接的用户信息
public slots:
    void thisReadData();//处理接收到的数据
};

#endif // MYTCPSOCKET_H
