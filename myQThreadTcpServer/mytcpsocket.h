#ifndef MYTCPSOCKET_H
#define MYTCPSOCKET_H

#include <QtNetwork>
#include <QtCore>
#include <QThread>
//继承QTcpSocket以处理接收到的数据
//实际应用中，应该是直接处理的、、
class myTcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit myTcpSocket(qintptr socketDescriptor,QObject *parent = 0);
    ~myTcpSocket();

signals:
    void readData(const int,const QString &,const quint16,const QByteArray &);//发送获得用户发过来的数据
    void sockDisConnect(const int ,const QString &,const quint16, QThread *);//NOTE:断开连接的用户信息，此信号必须发出！线程管理类根据信号计数的
public slots:
    void thisReadData();//处理接收到的数据
    void sentData(const QByteArray & ,const int);//发送信号的槽
    void disConTcp(int i);//断开信号
private:
    qintptr socketID;//保存id，== this->socketDescriptor()；但是this->socketDescriptor()客户端断开会被释放，
                        //断开信号用this->socketDescriptor()得不到正确值
    QMetaObject::Connection dis;
};

#endif // MYTCPSOCKET_H
