#ifndef TESTSOCKETS_H
#define TESTSOCKETS_H

#include <QObject>
#include <QtCore>
#include <QtNetwork>

class TestSockets : public QObject
{
    Q_OBJECT
public:
    explicit TestSockets(QObject *parent = 0);

signals:

public slots:

private:
    QList<QTcpSocket *> tcps;
};

#endif // TESTSOCKETS_H
