#include <QtCore/QCoreApplication>

#include "SimpleTcpClientExample.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QString strServerIP = "192.168.0.94";
    quint16 ui16ServerPort = 8888;

    SimpleTcpClientExample clsTestExample;
    clsTestExample.setServerInfo(strServerIP, ui16ServerPort);

    return a.exec();
}
