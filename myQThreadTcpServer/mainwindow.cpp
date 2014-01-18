#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MyQTheadTcpServer)
{
    ui->setupUi(this);
    combox = new QMap<QString, int>;
    tcpServer = new MyTcpServer(this);
    this->ui->txtport->setText("6666");
    this->ui->pushSent->setEnabled(false);

    connect(tcpServer,&MyTcpServer::sockDisConnect, this, &MainWindow::clintDisConnect);
    connect(tcpServer,&MyTcpServer::connectClient,this,&MainWindow::clintConnect);
    connect(tcpServer,&MyTcpServer::readData,this,&MainWindow::getData);

}

MainWindow::~MainWindow()
{
    delete combox;
    delete tcpServer;
    delete ui;
}

void MainWindow::getData(qintptr handle, QString ip, quint16 prot, QByteArray data)
{
    this->ui->textEdit->append(tr("From [ID：%1，IP：%2:%3] say ： %4").arg(handle).arg(ip).arg(prot).arg(QString(data)));
}

void MainWindow::clintConnect(qintptr handle, QString ip, quint16 prot)
{
    QString temp(tr("ID：%1，IP：%2:%3").arg(handle).arg(ip).arg(prot));
    this->ui->textEdit->append(tr("[%1]已连接").arg(temp));
    combox->insert(temp,handle);
    this->ui->comboBox->addItem(temp);
}

void MainWindow::clintDisConnect(qintptr handle, QString ip, quint16 prot)
{
     QString temp(tr("ID：%1，IP：%2:%3").arg(handle).arg(ip).arg(prot));
     this->ui->textEdit->append(tr("[%1]已断开").arg(temp));
     combox->remove(temp);
     this->ui->comboBox->removeItem(this->ui->comboBox->findText(temp));
}

void MainWindow::on_pushSent_clicked()
{
    QString data = this->ui->txtsentData->text();
    if (data.isEmpty())
    {
        return;
    }
    int handle = combox->find(this->ui->comboBox->currentText()).value();
    tcpServer->setData(data.toLatin1(),handle);
}

void MainWindow::on_pushStart_clicked()
{
    if (this->ui->pushStart->text() == "开始监听")
    {
       bool boltmp =  tcpServer->listen(QHostAddress::Any,this->ui->txtport->text().toInt());
       if (boltmp)
       {
           this->ui->txtport->setEnabled(false);
           this->ui->pushStart->setText("停止监听");
           this->ui->pushSent->setEnabled(true);
           this->ui->textEdit->append("监听成功！");
            qDebug() <<"MainWindow::on_pushStart_clicked THREAD IS：" <<QThread::currentThreadId();
       }
       else
       {
           qDebug() << tcpServer->errorString();
       }
    }
    else
    {
        tcpServer->close();
        tcpServer->clear();
        this->ui->txtport->setEnabled(true);
        this->ui->pushStart->setText("开始监听");
        this->ui->pushSent->setEnabled(false);
        this->ui->textEdit->append("停止成功！");
    }
}
