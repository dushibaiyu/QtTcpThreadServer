#include "mytcpserver.h"

MyTcpServer::MyTcpServer(QObject *parent) :
    QTcpServer(parent)
{
    tcpClient = new  QMap<int,QTcpSocket *>;
}

MyTcpServer::~MyTcpServer()
{
    delete tcpClient;
}

void MyTcpServer::incomingConnection(qintptr socketDescriptor)
{
    QTcpSocket * tcpTemp = new QTcpSocket;
    QThread * thread = new QThread;
    tcpTemp->setSocketDescriptor(socketDescriptor);
    connect(tcpTemp,&QTcpSocket::readyRead,[](){
                                                emit sentData(socketDescriptor,tcpTemp->peerAddress().toString(),tcpTemp->peerPort(),tcpTemp->readAll());
                                                });
    connect(tcpTemp,&QTcpSocket::disconnected,tcpTemp,&QTcpSocket::deleteLater);
    connect(tcpTemp,&QTcpSocket::disconnected,thread,&QThread::quit);
    connect(tcpTemp,&QTcpSocket::disconnected,thread,&QThread::deleteLater);
    tcpTemp->moveToThread(thread);
    tcpClient->insert(socketDescriptor,tcpTemp);
}

void MyTcpServer::setData(QArrayData &data, int handle)
{

}
