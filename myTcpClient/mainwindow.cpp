#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    tcpClient = new QTcpSocket(this);
    ui->pushSent->setEnabled(false);
    tcpClient->abort();
    connect(tcpClient,&QTcpSocket::readyRead,
            [this](){this->ui->textEdit->append(tr("Server Say：%1").arg(QString(this->tcpClient->readAll())));});
    connect(tcpClient,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(ReadError(QAbstractSocket::SocketError)));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete tcpClient;
}

void MainWindow::on_pushConnect_clicked()
{
    qDebug() << "点击连接：" ;
    if ("连接" == this->ui->pushConnect->text())
    {
        QString ipAdd(this->ui->txtIp->text()), portd(this->ui->txtPort->text());
        if (ipAdd.isEmpty() || portd.isEmpty())
        {
            this->ui->textEdit->append("请输入IP和端口!");
            return;
        }
        tcpClient->connectToHost(ipAdd,portd.toInt());
        if (tcpClient->waitForConnected(1000))
        {
            ui->pushConnect->setText("断开");
            ui->textEdit->append("连接服务器成功");
            ui->pushSent->setEnabled(true);
            this->ui->txtIp->setEnabled(false);
            this->ui->txtPort->setEnabled(false);
        }
    }
    else
    {
        tcpClient->disconnectFromHost();
        if (tcpClient->state() == QAbstractSocket::UnconnectedState || tcpClient->waitForDisconnected(1000) )
        {
            ui->pushConnect->setText("连接");
            ui->textEdit->append("断开服务器");
            ui->pushSent->setEnabled(false);
            this->ui->txtIp->setEnabled(true);
            this->ui->txtPort->setEnabled(true);
        }
    }
}

void MainWindow::on_pushSent_clicked()
{
    qDebug() << "点击发送：" ;
    QString data = this->ui->txtData->text();
    if (data.isEmpty())
    {
        return ;
    }
    tcpClient->write(data.toLatin1());
    ui->textEdit->append(tr("Say：%1").arg(data));
}

void MainWindow::ReadError(QAbstractSocket::SocketError)
{
    tcpClient->disconnectFromHost();
    ui->pushConnect->setText("连接");
    ui->textEdit->append(tr("连接出错：%1").arg(tcpClient->errorString()));
    ui->pushSent->setEnabled(false);
    this->ui->txtIp->setEnabled(true);
    this->ui->txtPort->setEnabled(true);
}
