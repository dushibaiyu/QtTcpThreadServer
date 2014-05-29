#include "mytcpsocket.h"

myTcpSocket::myTcpSocket(qintptr socketDescriptor, QObject *parent) : //构造函数在主线程执行，lambda在子线程
    QTcpSocket(parent),socketID(socketDescriptor)
{
    this->setSocketDescriptor(socketDescriptor);
//    cov = new PinYinConvert;
    connect(this,&myTcpSocket::readyRead,
            [&](){
        QTime time;
        time.start();
        QString str(this->readAll());

        QElapsedTimer tm;
        tm.start();
        while(tm.elapsed() < 100)
        {}

        qDebug() << this->peerAddress().toString() << ":" << this->peerPort() << "@"<<  time.elapsed();
        this->write(str.toUtf8());
    });
    dis = connect(this,&myTcpSocket::disconnected,
            [&](){
                emit sockDisConnect(socketID,this->peerAddress().toString(),this->peerPort(),QThread::currentThread());//发送断开连接的用户信息
            });

}

myTcpSocket::~myTcpSocket()
{
//    delete cov;
}


void myTcpSocket::sentData(const QByteArray &data, const int id)
{
    if(id == socketID)
    {
        write(data);
    }
}

void myTcpSocket::disConTcp(int i)
{
    if (i == socketID)
    {
        this->disconnectFromHost();
    }
    else if (i == -1) //-1为全部断开
    {
        disconnect(dis); //先断开连接的信号槽，防止二次析构
        this->disconnectFromHost();

        this->~myTcpSocket();//析构此函数
    }
}
