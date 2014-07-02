#include "testsockets.h"

TestSockets::TestSockets(QObject *parent) :
    QObject(parent)
{
    QThread * th;
    th = new QThread(this);
    th->start();
    for (int i = 1; i <= 500 ; ++i)
    {
        TcpSocket * tmp = new TcpSocket;
        connect(this,&TestSockets::connectsocket,tmp,&TcpSocket::cth);
        connect(this,&TestSockets::sent,tmp,&TcpSocket::sent);
        connect(tmp,&TcpSocket::sentdata,this,&TestSockets::sentstring);
        connect(tmp,&TcpSocket::senterro,this,&TestSockets::sentstring);
        connect(this,&TestSockets::disconnectsoc,tmp,&TcpSocket::disconnectFromHost);
        if (i %20 == 0)
        {
            th = new QThread(this);
            th->start();
        }
        tmp->moveToThread(th);
    }
}

TestSockets::~TestSockets()
{
    qDeleteAll(tcps);
}
