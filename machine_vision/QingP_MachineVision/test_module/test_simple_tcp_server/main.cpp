#include <QtCore/QCoreApplication>

#include "SimpleTcpServerExample.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    quint16 ui16ServerPort = 8888;

    SimpleTcpServerExample clsTestExample;
    clsTestExample.setServerPort(ui16ServerPort);

    QList<SimpleTcpServer::SimpleTcpClientInfo*> lstpstClientInfo = clsTestExample.m_pclsSimpleTcpServer->getClientList();

    if (lstpstClientInfo.size() > 0)
    {
        clsTestExample.m_pclsSimpleTcpServer->sendMessage(
            lstpstClientInfo[0]->strClientIP, \
            lstpstClientInfo[0]->ui16ClientPort, \
            "client connect server successfully!"
        );
    }

    return a.exec();
}
