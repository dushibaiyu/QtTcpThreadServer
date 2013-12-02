#include "mytcpserver.h"

MyTcpServer::MyTcpServer(QObject *parent) :
    QTcpServer(parent)
{
     tcpClient = new  QMap<int,myTcpSocket *>;
     qDebug() <<"MyTcpServer::MyTcpServer THREAD IS：" <<QThread::currentThreadId();
}

MyTcpServer::~MyTcpServer()
{
    delete tcpClient;
}

void MyTcpServer::incomingConnection(qintptr socketDescriptor)
{
    myTcpSocket * tcpTemp = new myTcpSocket(socketDescriptor);
    QThread * thread = new QThread(tcpTemp);//把线程的父类设为连接的，防止内存泄漏
    //可以信号连接信号的，我要捕捉线程ID就独立出来函数了，使用中还是直接连接信号效率应该有优势
    connect(tcpTemp,&myTcpSocket::readData,this,&MyTcpServer::readDataSlot);//接受到数据
    connect(tcpTemp,&myTcpSocket::sockDisConnect,this,&MyTcpServer::sockDisConnectSlot);//断开连接的处理，从列表移除，并释放断开的Tcpsocket
    connect(this,&MyTcpServer::sentData,tcpTemp,&myTcpSocket::sentData);//发送数据
    connect(tcpTemp,&myTcpSocket::disconnected,thread,&QThread::quit);//断开连接时线程退出
    tcpTemp->moveToThread(thread);//把tcp类移动到新的线程
    thread->start();//线程开始运行

    tcpClient->insert(socketDescriptor,tcpTemp);//插入到连接信息中
    qDebug() <<"incomingConnection THREAD IS：" <<QThread::currentThreadId();

    //发送连接信号
    emit connectClient(tcpTemp->socketDescriptor(),tcpTemp->peerAddress().toString(),tcpTemp->peerPort());

}

void MyTcpServer::setData(const QByteArray &data, const int handle)
{
    emit sentData(data,handle);
}

void MyTcpServer::sockDisConnectSlot(int handle, QString ip, quint16 prot)
{
    qDebug() <<"MyTcpServer::sockDisConnectSlot thread is:" << QThread::currentThreadId();
    myTcpSocket * tcp = tcpClient->value(handle);
    tcpClient->remove(handle);//连接管理中移除断开连接的socket
    delete tcp;//释放断开连接的资源、、子对象线程也会释放
    emit sockDisConnect(handle,ip,prot);
}

void MyTcpServer::readDataSlot(const int handle, const QString &ip, const quint16 prot, const QByteArray &data)
{
    qDebug() <<"MyTcpServer::readDataSlot() thread is:" << QThread::currentThreadId();
    emit readData(handle,ip,prot,data);
}
