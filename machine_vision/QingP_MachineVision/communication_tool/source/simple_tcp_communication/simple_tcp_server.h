#ifndef SIMPLE_TCP_SERVER_H
#define SIMPLE_TCP_SERVER_H

#include "../../../communication_tool_global.h"

#include <QTcpSocket>
#include <QString>
#include <QThread>
#include <QList>

#include "simple_tcp_client.h"

class COMMUNICATION_TOOL_EXPORT SimpleTcpServer: public QTcpSocket
{
    Q_OBJECT
public:
    SimpleTcpServer(uint uiServerID = 0, QString strServerName = "", QObject* parent = nullptr);
    ~SimpleTcpServer();

private:
    uint m_uiServerID = 0;
    QString m_strServerName = "";

    int m_iClientCount = 0;
    QList<SimpleTcpClient*> m_lstpclsClientList;
    
signals:
    
};

#endif
