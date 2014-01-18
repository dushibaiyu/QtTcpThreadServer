#include "mytcpsocket.h"

myTcpSocket::myTcpSocket(qintptr socketDescriptor, QObject *parent) : //构造函数在主线程执行，lambda在子线程
    QTcpSocket(parent),socketID(socketDescriptor)
{
    this->setSocketDescriptor(socketDescriptor);
    //转换系统信号到我们需要发送的数据、、直接用lambda表达式了，qdebug是输出线程信息
    connect(this,&myTcpSocket::readyRead,this,&myTcpSocket::thisReadData); //连接到收到数据的处理函数
    connect(this,&myTcpSocket::readyRead, //转换收到的信息，发送信号
            [this](){
                qDebug() << socketID << " myTcpSocket::myTcpSocket lambda readData thread is:" << QThread::currentThreadId();
                emit readData(socketID,this->peerAddress().toString(),this->peerPort() ,this->readAll());//发送用户发过来的数据
            });
    dis = connect(this,&myTcpSocket::disconnected, //断开连接的信号转换
            [this](){
                qDebug() << socketID <<"myTcpSocket::myTcpSocket lambda sockDisConnect thread is:" << QThread::currentThreadId();
                emit sockDisConnect(socketID,this->peerAddress().toString(),this->peerPort(),QThread::currentThread());//发送断开连接的用户信息
            });

    qDebug() << this->socketDescriptor() << " " << this->peerAddress().toString()
                << " " << this->peerPort() << "myTcpSocket::myTcpSocket thread is " <<QThread::currentThreadId();
}

myTcpSocket::~myTcpSocket()
{
    qDebug() << QString("~myTcpSocket() at %1").arg(socketID) ;
}

void myTcpSocket::thisReadData()//收到数据的处理函数
{
    qDebug() << socketID << " " << this->peerAddress().toString()
                << " " << this->peerPort() << "  myTcpSocket::thisReadData thread is " <<QThread::currentThreadId();
}

void myTcpSocket::sentData(const QByteArray &data, const int id)
{
    //如果是服务器判断好，直接调用write会出现跨线程的现象，所以服务器用广播，每个连接判断是否是自己要发送的信息。
    if(id == socketID)//判断是否是此socket的信息
    {
        qDebug() << "myTcpSocket::sentData" << QThread::currentThreadId();
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
        qDebug() <<QString("disconnectFromHost() at %1").arg(this->socketID);
        this->~myTcpSocket();//析构此函数
    }
}
