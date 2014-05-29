#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mytcpserver.h"

namespace Ui {
class MyQTheadTcpServer;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void getData(qintptr handle, QString ip, quint16 prot, QByteArray data);//接受数据
    void clintConnect(qintptr handle, QString ip, quint16 prot);//有新用户连接
    void clintDisConnect(qintptr handle, QString ip, quint16 prot);//用户断开连接

    //直接写的点击响应事件
    void on_pushStart_clicked();
    void on_pushSent_clicked();
private:
    Ui::MyQTheadTcpServer * ui;
    MyTcpServer * tcpServer;
    QMap<QString, int> * combox;
};

#endif // MAINWINDOW_H
