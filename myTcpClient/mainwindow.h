#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QtNetwork>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void ReadError(QAbstractSocket::SocketError);

    void on_pushSent_clicked();
    void on_pushConnect_clicked();
    void on_timeBut_clicked();

private:
    Ui::MainWindow * ui;
    QTcpSocket * tcpClient;
    QTimer tm;
    QStringList list;
};

#endif // MAINWINDOW_H
