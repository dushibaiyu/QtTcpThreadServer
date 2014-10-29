#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QTcpSocket>
#include <QTime>


class TcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit TcpSocket(QObject *parent = 0);

signals:
    void sentdata(const QString & str);
    void senterro(const QString & erro);
public slots:
    void cth(const QString &host, int port);
    void sent();

    void ReadError(QAbstractSocket::SocketError);

protected:
    QTime tm;
};

#endif // TCPSOCKET_H
