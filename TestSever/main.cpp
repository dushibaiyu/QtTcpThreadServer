#include <QCoreApplication>
#include "mytcpserver.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    MyTcpServer ser;
    ser.listen(QHostAddress::Any,6666);

    return a.exec();
}
