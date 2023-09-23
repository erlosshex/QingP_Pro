#ifndef SIMPLE_TCP_SERVER_H
#define SIMPLE_TCP_SERVER_H

#include "../../communication_tool_global.h"

#include <QTcpServer>
#include <QTcpSocket>
#include <QString>
#include <QThread>
#include <QList>
#include <QMutex>

class COMMUNICATION_TOOL_EXPORT SimpleTcpServer: public QTcpServer
{
    Q_OBJECT
public:
    struct SimpleTcpClientInfo
    {
        QString strClientIP = "";
        quint16 ui16ClientPort = 0;
        QString strClientName = "";
        uint uiClientID = 0;
        QTcpSocket* pTcpSocket = nullptr;
    };

    SimpleTcpServer(uint uiServerID = 0, QString strServerName = "", QObject* parent = nullptr);
    ~SimpleTcpServer();

    QString getServerName();
    uint getServerID();
    void getServerInfo(QString& strServerName, uint& uiServerID);

    void setServerPort(quint16 ui16Port);

    quint16 getServerPort();

    QList<SimpleTcpClientInfo*> getClientList();

    void openServer();

    void closeServer();

    void sendMessage(QString strClientIP, quint16 ui16ClientPort, QString strMessage);

private:
    uint m_uiServerID = 0;
    QString m_strServerName = "";

    quint16 m_ui16ServerPort = 8000;

    int m_iClientCount = 0;
    QList<SimpleTcpClientInfo*> m_lstpstClientInfo;

    QMutex m_mtxNewConnection;
    
signals:
    void signal_readyReadFromOneClient(uint uiClientID, QString strClientIP, quint16 ui16ClientPort, QString strClientName, QByteArray baData);

private slots:
    void slot_newConnection();
};

#endif
