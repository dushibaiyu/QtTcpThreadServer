#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    tcpClient = new QTcpSocket(this);
    ui->pushSent->setEnabled(false);
    this->ui->timeBut->setEnabled(false);
    tcpClient->abort();
    connect(tcpClient,&QTcpSocket::readyRead,
            [&](){this->ui->textEdit->append(tr("%1 Server Say：%2").arg(QTime::currentTime().toString("hh:mm:ss.zzz")).arg(QString(this->tcpClient->readAll())));});
    connect(tcpClient,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(ReadError(QAbstractSocket::SocketError)));
    connect(&tm,&QTimer::timeout,[&](){
            int i = qrand() % 6;
            this->ui->textEdit->append(tr("%1 Timer Sent: %2").arg(QTime::currentTime().toString("hh:mm:ss.zzz")).arg(list.at(i)));
            tcpClient->write(list.at(i).toUtf8());
    });
    connect(tcpClient,&QTcpSocket::disconnected,[](){qDebug()<< "123333" ;});
    list << "我是谁?" << "渡世白玉" << "hello" << "哈哈哈哈哈" << "你是坏蛋!" <<  "测试一下下了" << "不知道写什么" ;
    QTime time;
    time= QTime::currentTime();
    qsrand(time.msec()+time.second()*1000);
    this->ui->txtIp->setText("127.0.0.1");
    this->ui->txtPort->setText("6666");
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
            this->ui->timeBut->setEnabled(true);
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
            tm.stop();
            this->ui->timeBut->setEnabled(false);
            this->ui->lineEdit->setEnabled(true);
            this->ui->timeBut->setText("启动定时");
        }
    }
}

void MainWindow::on_pushSent_clicked()
{
    qDebug() << "点击发送：" ;
    QString data = this->ui->txtData->toPlainText();//->text();
    if (data.isEmpty())
    {
        return ;
    }
    tcpClient->write(data.toUtf8());
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
    tm.stop();
    this->ui->timeBut->setEnabled(false);
    this->ui->lineEdit->setEnabled(true);
    this->ui->timeBut->setText("启动定时");
}

void MainWindow::on_timeBut_clicked()
{
    if (this->ui->lineEdit->text().isEmpty())
    {
        this->ui->textEdit->append("请输入时间：");
        return;
    }
    if ("启动定时" == this->ui->timeBut->text())
    {
        int h = this->ui->lineEdit->text().toInt();
        h = h*1000;
        tm.start(h);
        this->ui->lineEdit->setEnabled(false);
        this->ui->timeBut->setText("停止定时");
    }
    else
    {
        tm.stop();
        this->ui->timeBut->setText("启动定时");
        this->ui->lineEdit->setEnabled(true);
    }
}
