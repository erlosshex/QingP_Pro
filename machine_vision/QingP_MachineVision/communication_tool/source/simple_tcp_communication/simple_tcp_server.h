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
    };

    SimpleTcpServer(uint uiServerID = 0, QString strServerName = "", QObject* parent = nullptr);
    ~SimpleTcpServer();

private:
    uint m_uiServerID = 0;
    QString m_strServerName = "";

    int m_iClientCount = 0;
    QList<SimpleTcpClientInfo> m_lstpclsClientInfo;
    
signals:
    
};

#endif
