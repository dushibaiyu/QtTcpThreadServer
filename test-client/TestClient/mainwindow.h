#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include "testsockets.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void con(const QString & str, int port);
    void discon();

public slots:
    void on_pushConnect_clicked();
    void on_timeBut_clicked();

//    void setdata(const QString & str);
private:
    Ui::MainWindow * ui;
    TestSockets * tcps = nullptr;
    QTimer tm;
};

#endif // MAINWINDOW_H
