#ifndef TCPSOCKETTHREAD_H
#define TCPSOCKETTHREAD_H

#include <QThread>
#include <QtCore>
#include <QtNetwork>

class TcpSocketThread : public QThread
{
    Q_OBJECT
public:
    explicit TcpSocketThread(qintptr socketDescriptor,QObject *parent = 0);
    void run();
signals:
    void readData(int, QString, quint16, QByteArray);//发送获得用户发过来的数据
    void sockDisConnect(int , QString , quint16 );//断开连接的用户信息
    void socketConnect(int , QString , quint16 );
    void writeDataSignal(QByteArray data);
public slots:
     void writeData(QByteArray data);
     void thisReadData();//处理接收到的数据
private:
     qintptr socketDescriptor;
     QTcpSocket * scoketd;
};

#endif // TCPSOCKETTHREAD_H
