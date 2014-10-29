#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->ui->timeBut->setEnabled(false);
    this->ui->txtIp->setText("127.0.0.1");
    this->ui->txtPort->setText("6666");
}

MainWindow::~MainWindow()
{
    delete ui;
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
            if(tcps == nullptr)
            {
                tcps = new TestSockets;
                connect(this,&MainWindow::con,tcps,&TestSockets::connectsocket);
                connect(this,&MainWindow::discon,tcps,&TestSockets::disconnectsoc);
                connect(tcps,&TestSockets::sentstring,[&](const QString & str){this->ui->textEdit->append(str);});
                connect(&tm,&QTimer::timeout,tcps,&TestSockets::sent);
            }
            emit con(this->ui->txtIp->text(),this->ui->txtPort->text().toInt());
            ui->pushConnect->setText("断开");
            ui->textEdit->append("连接服务器成功");
            this->ui->txtIp->setEnabled(false);
            this->ui->txtPort->setEnabled(false);
            this->ui->timeBut->setEnabled(true);

    }
    else
    {
        ui->pushConnect->setText("连接");
        ui->textEdit->append("断开服务器");
        this->ui->txtIp->setEnabled(true);
        this->ui->txtPort->setEnabled(true);
        emit this->discon();
        tm.stop();
        this->ui->timeBut->setEnabled(false);
        this->ui->lineEdit->setEnabled(true);
        this->ui->timeBut->setText("启动定时");

    }
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
