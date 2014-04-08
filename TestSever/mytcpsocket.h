#ifndef MYTCPSOCKET_H
#define MYTCPSOCKET_H

#include <QtNetwork>
#include <QtCore>
//#include "pinyinconvert.h"

class myTcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit myTcpSocket(qintptr socketDescriptor,QObject *parent = 0);
    ~myTcpSocket();

signals:
    void readData(const int,const QString &,const quint16,const QByteArray &);
    void sockDisConnect(const int ,const QString &,const quint16, QThread *);//NOTE:断开连接的用户信息，此信号必须发出！线程管理类根据信号计数的
public slots:
    void sentData(const QByteArray & ,const int);//发送信号的槽
    void disConTcp(int i);
private:
//    PinYinConvert * cov;
    qintptr socketID;
    QMetaObject::Connection dis;
};

#endif // MYTCPSOCKET_H
