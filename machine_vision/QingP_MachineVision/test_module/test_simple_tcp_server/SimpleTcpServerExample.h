#pragma once

#include "../../communication_tool/source/simple_tcp_communication/simple_tcp_server.h"

#include <QObject>
#include <QString>
#include <QThread>
#include <QDebug>

class SimpleTcpServerExample : public QObject
{
	Q_OBJECT
public:
	SimpleTcpServerExample();
	~SimpleTcpServerExample();

	void setServerPort(quint16 ui16Port);
	
	SimpleTcpServer* m_pclsSimpleTcpServer = nullptr;

private slots:
	void slot_showRecvMessage(uint uiClientID, QString strClientIP, quint16 ui16ClientPort, QString strClientName, QByteArray baData);
};
