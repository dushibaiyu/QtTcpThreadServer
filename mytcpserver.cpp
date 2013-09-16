#include "mytcpserver.h"

MyTcpServer::MyTcpServer(QObject *parent) :
    QTcpServer(parent)
{
    tcpClient = new  QMap<int,QTcpSocket *>;
}
