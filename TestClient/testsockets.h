#ifndef TESTSOCKETS_H
#define TESTSOCKETS_H

#include <QObject>
#include <QtCore>
#include <QtNetwork>
#include "tcpsocket.h"

class TestSockets : public QObject
{
    Q_OBJECT
public:
    explicit TestSockets(QObject *parent = 0);
    ~TestSockets();

signals:
    void sent();
    void connectsocket(const QString & ip, int port);
    void sentstring(const QString & str);
    void disconnectsoc();
public slots:

private:
    QList<TcpSocket *> tcps;
};

#endif // TESTSOCKETS_H
