#include "mytcpserver.h"

MyTcpServer::MyTcpServer(QObject *parent) :
    QTcpServer(parent)
{
    tcpClient = new  QMap<int,TcpSocketThread *>;
//    tcpClient = new  QMap<int,myTcpSocket *>;
     qDebug() <<"MyTcpServer::MyTcpServer THREAD IS：" <<QThread::currentThreadId();
}

MyTcpServer::~MyTcpServer()
{
    delete tcpClient;
}

void MyTcpServer::incomingConnection(qintptr socketDescriptor)
{
    //moveToThread方法不可用，还是在一个线程中。
//    QThread * thread = new QThread(this);
//    myTcpSocket * tcpTemp = new myTcpSocket(socketDescriptor);
//    connect(tcpTemp,&myTcpSocket::readData,this,&MyTcpServer::readDataSlot);
//    connect(tcpTemp,&myTcpSocket::sockDisConnect,this,&MyTcpServer::sockDisConnectSlot);
//    connect(tcpTemp,&myTcpSocket::disconnected,tcpTemp,&myTcpSocket::deleteLater);
//    connect(tcpTemp,&myTcpSocket::disconnected,thread,&QThread::quit);
//    connect(tcpTemp,&myTcpSocket::disconnected,thread,&QThread::deleteLater);
//    tcpTemp->moveToThread(thread);
//    tcpClient->insert(socketDescriptor,tcpTemp);
//    qDebug() <<"incomingConnection THREAD IS：" <<QThread::currentThreadId();
//     emit connectClient(tcpTemp->socketDescriptor(),tcpTemp->peerAddress().toString(),tcpTemp->peerPort());

    qDebug() <<"MyTcpServer::incomingConnection thread is:" << QThread::currentThreadId();
    TcpSocketThread * tcpTemp = new TcpSocketThread(socketDescriptor,this);
    //直接连接到server的槽，曹函数在主线程中。
    connect(tcpTemp,&TcpSocketThread::readData,this,&MyTcpServer::readDataSlot);
    //lamdba表达式在子线程中
    connect(tcpTemp,&TcpSocketThread::readData,//this,&MyTcpServer::readDataSlot);
                    [this](int handle, QString ip, quint16 prot, QByteArray data){
                        qDebug() <<"MyTcpServer::incomingConnection lambda readData thread is:" << QThread::currentThreadId();
                        emit readData(handle,ip,prot,data);
                    });
    connect(tcpTemp,&TcpSocketThread::sockDisConnect,//this,&MyTcpServer::sockDisConnectSlot);
                    [this](int handle, QString ip, quint16 prot){
                        qDebug() <<"MyTcpServer::incomingConnection lambda sockDisConnect thread is:" << QThread::currentThreadId();
                        tcpClient->remove(handle);
                        emit sockDisConnect(handle,ip,prot);
                    });
    connect(tcpTemp,&TcpSocketThread::socketConnect,//this,&MyTcpServer::socketConnectSolt);
                    [this](int handle, QString ip, quint16 prot){
                        qDebug() <<"MyTcpServer::incomingConnection lambda socketConnect thread is:" << QThread::currentThreadId();
                        emit connectClient(handle,ip,prot);
                    });


    tcpClient->insert(socketDescriptor,tcpTemp);
    tcpTemp->start();

}

void MyTcpServer::setData(QByteArray data, int handle)
{
    TcpSocketThread * tmp = tcpClient->find(handle).value();//查找到需要向哪个用户发送消息
     tmp->writeData(data);//向用户发送消息
    qDebug() <<"MyTcpServer::setData THREAD IS：" <<QThread::currentThreadId();
//    connect(this,&MyTcpServer::sentData,tmp,&TcpSocketThread::writeData);
//    emit sentData(data);// 同样需要跨线程、、唉、


//    myTcpSocket * tmp = tcpClient->find(handle).value();//查找到需要向哪个用户发送消息
//    tmp->write(data);
}

//void MyTcpServer::sockDisConnectSlot(int handle, QString ip, quint16 prot)
//{
//     qDebug() <<"MyTcpServer::sockDisConnectSlot thread is:" << QThread::currentThreadId();
//    tcpClient->remove(handle);
//    emit sockDisConnect(handle,ip,prot);
//}

void MyTcpServer::readDataSlot(int handle, QString ip, quint16 prot, QByteArray data)
{
    qDebug() <<"MyTcpServer::readDataSlot() thread is:" << QThread::currentThreadId();
}
