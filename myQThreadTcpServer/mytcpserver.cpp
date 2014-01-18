#include "mytcpserver.h"
#include "threadhandle.h"

MyTcpServer::MyTcpServer(QObject *parent) :
    QTcpServer(parent)
{
     tcpClient = new  QMap<int,myTcpSocket *>;
     qDebug() <<"MyTcpServer::MyTcpServer THREAD IS：" <<QThread::currentThreadId();
     ThreadHandle::getClass().initThreadType();//初始化线程管理类
}

MyTcpServer::~MyTcpServer()
{
    emit this->sentDisConnect(-1);

    delete tcpClient;
}

void MyTcpServer::incomingConnection(qintptr socketDescriptor) //多线程喜碧在此函数里捕获新连接
{
    myTcpSocket * tcpTemp = new myTcpSocket(socketDescriptor);

    //可以信号连接信号的，我要捕捉线程ID就独立出来函数了，使用中还是直接连接信号效率应该有优势
    connect(tcpTemp,&myTcpSocket::readData,this,&MyTcpServer::readDataSlot);//接受到数据
    connect(tcpTemp,&myTcpSocket::sockDisConnect,this,&MyTcpServer::sockDisConnectSlot);//NOTE:断开连接的处理，从列表移除，并释放断开的Tcpsocket，此槽必须实现，线程管理计数也是考的他
    connect(this,&MyTcpServer::sentData,tcpTemp,&myTcpSocket::sentData);//发送数据
    connect(this,&MyTcpServer::sentDisConnect,tcpTemp,&myTcpSocket::disConTcp);//断开信号

    tcpTemp->moveToThread(ThreadHandle::getClass().getThread());//把tcp类移动到新的线程，从线程管理类中获取


    tcpClient->insert(socketDescriptor,tcpTemp);//插入到连接信息中
    qDebug() <<"incomingConnection THREAD IS：" <<QThread::currentThreadId();

    //发送连接信号
    emit connectClient(tcpTemp->socketDescriptor(),tcpTemp->peerAddress().toString(),tcpTemp->peerPort());

}

void MyTcpServer::setData(const QByteArray &data, const int handle)
{
    emit sentData(data,handle);
}

void MyTcpServer::sockDisConnectSlot(int handle,const QString & ip, quint16 prot,QThread * th)
{
    qDebug() <<"MyTcpServer::sockDisConnectSlot thread is:" << QThread::currentThreadId();
    myTcpSocket * tcp = tcpClient->value(handle);
    tcpClient->remove(handle);//连接管理中移除断开连接的socket
    ThreadHandle::getClass().removeThread(th); //告诉线程管理类那个线程里的连接断开了
    delete tcp;//释放断开连接的资源
    emit sockDisConnect(handle,ip,prot);
}

void MyTcpServer::readDataSlot(const int handle, const QString &ip, const quint16 prot, const QByteArray &data)
{
    qDebug() <<"MyTcpServer::readDataSlot() thread is:" << QThread::currentThreadId();
    emit readData(handle,ip,prot,data);
}

void MyTcpServer::clear()
{
    emit this->sentDisConnect(-1);
    ThreadHandle::getClass().clear();
    tcpClient->clear();
}
