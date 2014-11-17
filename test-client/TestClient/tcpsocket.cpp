#include "tcpsocket.h"
#include <QUuid>
#include <QDebug>
#include <QNetworkProxy>

TcpSocket::TcpSocket(QObject *parent) :
    QTcpSocket(parent)
{
    connect(this,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(ReadError(QAbstractSocket::SocketError)));
    connect(this,&TcpSocket::readyRead,[&](){QString str(QString::number(this->peerPort())); str += " : ";
    str += this->readAll(); emit this->sentdata(str);});

    this->setProxy(QNetworkProxy(QNetworkProxy::Socks5Proxy,"127.0.0.1",6666));
}

void TcpSocket::cth(const QString &host, int port)
{
    this->connectToHost(host,port);
    this->waitForConnected(5000);
}

void TcpSocket::sent()
{
    if (this->state() == QAbstractSocket::ConnectedState)
    {
        this->write(QUuid::createUuid().toByteArray());
        tm.start();
    }
}

void TcpSocket::ReadError(QAbstractSocket::SocketError)
{
    QString str(QString::number(this->peerPort()));
    str += " : erro : ";
    str += this->errorString();
    emit senterro(str);
}
