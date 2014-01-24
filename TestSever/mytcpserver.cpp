#include "mytcpserver.h"
#include "threadhandle.h"

MyTcpServer::MyTcpServer(QObject *parent) :
    QTcpServer(parent)
{
     tcpClient = new  QMap<int,myTcpSocket *>;
     ThreadHandle::getClass().initThreadType(ThreadHandle::THREADSIZE,3);//TODO: 初始化线程管理类,现在设置为开3个固定线程
     this->setMaxPendingConnections(1000);
}

MyTcpServer::~MyTcpServer()
{
    emit this->sentDisConnect(-1);

    delete tcpClient;
}

void MyTcpServer::incomingConnection(qintptr socketDescriptor) //多线程喜碧在此函数里捕获新连接
{
    myTcpSocket * tcpTemp = new myTcpSocket(socketDescriptor);

    connect(tcpTemp,&myTcpSocket::sockDisConnect,this,&MyTcpServer::sockDisConnectSlot);//NOTE:断开连接的处理，从列表移除，并释放断开的Tcpsocket，此槽必须实现，线程管理计数也是考的他
    connect(this,&MyTcpServer::sentDisConnect,tcpTemp,&myTcpSocket::disConTcp);//断开信号
    tcpTemp->moveToThread(ThreadHandle::getClass().getThread());//把tcp类移动到新的线程，从线程管理类中获取

    tcpClient->insert(socketDescriptor,tcpTemp);//插入到连接信息中

    emit connectClient(tcpTemp->socketDescriptor(),tcpTemp->peerAddress().toString(),tcpTemp->peerPort());

    qDebug() << "Have New Connection! " << tcpTemp->peerAddress().toString() << ":" << tcpTemp->peerPort();
}

void MyTcpServer::sockDisConnectSlot(int handle,const QString & ip, quint16 prot,QThread * th)
{
    myTcpSocket * tcp = tcpClient->value(handle);
    tcpClient->remove(handle);//连接管理中移除断开连接的socket
    ThreadHandle::getClass().removeThread(th); //告诉线程管理类那个线程里的连接断开了
    delete tcp;//释放断开连接的资源
    emit sockDisConnect(handle,ip,prot);
}


void MyTcpServer::clear()
{
    emit this->sentDisConnect(-1);
    ThreadHandle::getClass().clear();
    tcpClient->clear();
}
