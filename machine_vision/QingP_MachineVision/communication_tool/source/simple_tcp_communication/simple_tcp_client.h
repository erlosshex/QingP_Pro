#ifndef SIMPLE_TCP_CLIENT_H
#define SIMPLE_TCP_CLIENT_H

#include "../../communication_tool_global.h"

#include <QTcpSocket>
#include <QString>
#include <QThread>

class COMMUNICATION_TOOL_EXPORT SimpleTcpClient : public QTcpSocket
{
    Q_OBJECT
public:
    SimpleTcpClient(uint uiClientID = 0, QString strClientName = "", QObject* parent = nullptr);
    ~SimpleTcpClient();

    uint getClientID();
    QString getClientName();

    QString getServerIP();
    quint16 getServerPort();
    void getServerInfo(QString& strServerIP, quint16& ui16ServerPort);

    void setServerIP(QString strServerIP);
    void setServerPort(quint16 ui16ServerPort);
    void setServerInfo(QString strServerIP, quint16 ui16ServerPort);

    void connectServer();

    void sendMessage(QString strMessage);

private:
    uint m_uiClientID = 0;
    QString m_strClientName = "";

    QString m_strServerIP = "";
    quint16 m_ui16ServerPort = 8000;

signals:
    void signal_clientReadData(uint uiClientID, QString strIP, uint uiPort, QByteArray baData);
    void signal_clientDisconnect(uint uiClientID, QString strIP, uint uiPort);

private slots:
    void slot_clientReadData();
    void slot_disconnect();
};

#endif
