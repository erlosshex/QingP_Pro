#ifndef SIMPLE_TCP_SERVER_H
#define SIMPLE_TCP_SERVER_H

#include "../../../communication_tool_global.h"

#include <QTcpServer>
#include <QTcpSocket>
#include <QString>
#include <QThread>
#include <QList>

class COMMUNICATION_TOOL_EXPORT SimpleTcpServer: public QTcpServer
{
    Q_OBJECT
public:
    struct SimpleTcpClientInfo
    {
        QString strClientIP = "";
        QString strClientName = "";
        uint uiClientID = 0;
        QTcpSocket* pTcpSocket = nullptr;
    };

    SimpleTcpServer(uint uiServerID = 0, QString strServerName = "", QObject* parent = nullptr);
    ~SimpleTcpServer();

    QString getServerName();
    uint getServerID();
    void getServerInfo(QString& strServerName, uint& uiServerID);

    QList<SimpleTcpClientInfo> getClientList();

    void openServer();

    void closeServer();


private:
    uint m_uiServerID = 0;
    QString m_strServerName = "";

    quint16 ui16ServerPort = 8000;

    int m_iClientCount = 0;
    QList<SimpleTcpClientInfo> m_lstpstClientInfo;
    
signals:
    
};

#endif
