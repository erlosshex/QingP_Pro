#pragma once

#include "../../communication_tool/source/simple_tcp_communication/simple_tcp_client.h"

#include <QObject>
#include <QString>
#include <QThread>
#include <QDebug>

class SimpleTcpClientExample : public QObject
{
	Q_OBJECT
public:
	SimpleTcpClientExample();
	~SimpleTcpClientExample();

	void setServerInfo(QString strServerIP, quint16 ui16ServerPort);

private:
	SimpleTcpClient* m_pclsSimpleTcpClient = nullptr;

private slots:
	void slot_showRecvMessage(uint uiClientID, QString strIP, uint uiPort, QByteArray baData);
};
