#ifndef MYTCPSOCKET_H
#define MYTCPSOCKET_H

#include <QtNetwork>
#include <QtCore>
#include <QThread>
//继承QTcpSocket以处理接收到的数据
class myTcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit myTcpSocket(qintptr socketDescriptor,QObject *parent = 0);

signals:
    void readData(const int,const QString &,const quint16,const QByteArray &);//发送获得用户发过来的数据
    void sockDisConnect(const int ,const QString &,const quint16 );//断开连接的用户信息
public slots:
    void thisReadData();//处理接收到的数据
    void sentData(const QByteArray & ,const int);//发送信号的槽
private:
    qintptr socketID;//保存id，== this->socketDescriptor()；但是this->socketDescriptor()客户端断开会被释放，
                        //断开信号用this->socketDescriptor()得不到正确值
};

#endif // MYTCPSOCKET_H
