#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include <QTcpServer>
#include <QtCore>

class MyTcpServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit MyTcpServer(QObject *parent = 0);
    ~MyTcpServer();
signals:
    void connectClient(int handle, QString & ip, int port );
    void disConClient(int handle, QString & ip, int port, QArrayData & data);
public slots:
    void setData(QVariant & data, int handle);
protected:
    void incomingConnection(qintptr socketDescriptor);
private:
    QMap<int,QTcpSocket *> * tcpClient;
};

#endif // MYTCPSERVER_H
