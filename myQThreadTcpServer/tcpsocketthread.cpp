#include "tcpsocketthread.h"

TcpSocketThread::TcpSocketThread(qintptr socketDescriptor,QObject *parent) :
    QThread(parent)
{
    this->socketDescriptor = socketDescriptor;
}

void TcpSocketThread::run()//在子线程中运行
{
    scoketd = new QTcpSocket;
    scoketd->setSocketDescriptor(socketDescriptor);
    connect(scoketd,&QTcpSocket::readyRead,this,&TcpSocketThread::thisReadData);
    connect(scoketd,&QTcpSocket::readyRead,
            [this](){
                qDebug() <<"TcpSocketThread::run lambda readData thread is:" << QThread::currentThreadId();
                emit readData(scoketd->socketDescriptor(),scoketd->peerAddress().toString(),scoketd->peerPort() ,scoketd->readAll());//发送用户发过来的数据
            });
    connect(scoketd,&QTcpSocket::disconnected,
            [this](){
                qDebug() <<"TcpSocketThread::run lambda sockDisConnect thread is:" << QThread::currentThreadId();
                emit sockDisConnect(scoketd->socketDescriptor(),scoketd->peerAddress().toString(),scoketd->peerPort());//发送断开连接的用户信息
            });
//    connect(this,&TcpSocketThread::writeDataSignal,scoketd
//            [this](QByteArray data)
//            {
//                if (scoketd != nullptr)
//                {
//                    qDebug() << "TcpSocketThread::writeDataSignal() thread is " <<QThread::currentThreadId();
//                    this->scoketd->write(data);
//                }
//            });//不能跨线程、、
    connect(scoketd,&QTcpSocket::disconnected,scoketd,&QTcpSocket::deleteLater);
    connect(scoketd,&QTcpSocket::disconnected,[this](){qDebug()<< QThread::currentThreadId()<<" Exit."; this->exit();});
    connect(scoketd,&QTcpSocket::disconnected,this,&TcpSocketThread::deleteLater);
    emit (socketConnect(scoketd->socketDescriptor(),scoketd->peerAddress().toString(),scoketd->peerPort()));
    this->exec();
}

void TcpSocketThread::thisReadData()//在主线程中运行
{
    qDebug() << " TcpSocketThread::thisReadData() thread is " <<QThread::currentThreadId();
}

void TcpSocketThread::writeData(QByteArray data)//在主线程中运行
{
//    if (scoketd != nullptr)
//    {
        qDebug() << "TcpSocketThread::writeData() thread is " <<QThread::currentThreadId();
//        scoketd->write(data);
//    }
    emit writeDataSignal(data);
}

