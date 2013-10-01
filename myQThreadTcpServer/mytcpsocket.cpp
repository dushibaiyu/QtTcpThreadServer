#include "mytcpsocket.h"

myTcpSocket::myTcpSocket(qintptr socketDescriptor, QObject *parent) :
    QTcpSocket(parent)
{
    this->setSocketDescriptor(socketDescriptor);
    connect(this,&myTcpSocket::readyRead,this,&myTcpSocket::thisReadData);
    connect(this,&myTcpSocket::readyRead,
            [this](){
                qDebug() <<"myTcpSocket::myTcpSocket lambda readData thread is:" << QThread::currentThreadId();
                emit readData(this->socketDescriptor(),this->peerAddress().toString(),this->peerPort() ,this->readAll());//发送用户发过来的数据
            });
    connect(this,&myTcpSocket::disconnected,
            [this](){
                qDebug() <<"myTcpSocket::myTcpSocket lambda sockDisConnect thread is:" << QThread::currentThreadId();
                emit sockDisConnect(this->socketDescriptor(),this->peerAddress().toString(),this->peerPort());//发送断开连接的用户信息
            });
    qDebug() << this->socketDescriptor() << " " << this->peerAddress().toString()
                << " " << this->peerPort() << "myTcpSocket::myTcpSocket thread is " <<QThread::currentThreadId();
}

void myTcpSocket::thisReadData()
{
    qDebug() << this->socketDescriptor() << " " << this->peerAddress().toString()
                << " " << this->peerPort() << "  myTcpSocket::thisReadData thread is " <<QThread::currentThreadId();
}
