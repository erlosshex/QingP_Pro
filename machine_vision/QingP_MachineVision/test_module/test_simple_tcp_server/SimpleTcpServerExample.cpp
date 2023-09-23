#include "SimpleTcpServerExample.h"

SimpleTcpServerExample::SimpleTcpServerExample()
{
	m_pclsSimpleTcpServer = new SimpleTcpServer(0, "TestServer");

	connect(m_pclsSimpleTcpServer, &SimpleTcpServer::signal_readyReadFromOneClient, this, &SimpleTcpServerExample::slot_showRecvMessage);
}

SimpleTcpServerExample::~SimpleTcpServerExample()
{
	if (nullptr != m_pclsSimpleTcpServer)
	{
		delete m_pclsSimpleTcpServer;
		m_pclsSimpleTcpServer = nullptr;
	}
}

void SimpleTcpServerExample::setServerPort(quint16 ui16Port)
{
	if (nullptr == m_pclsSimpleTcpServer)
	{
		return;
	}

	m_pclsSimpleTcpServer->setServerPort(ui16Port);

	m_pclsSimpleTcpServer->openServer();

	QThread::msleep(2000);
}

void SimpleTcpServerExample::slot_showRecvMessage(uint uiClientID, QString strClientIP, quint16 ui16ClientPort, QString strClientName, QByteArray baData)
{
	qDebug() << QString(20, '*');
	qDebug() << "Client ID : " << uiClientID;
	qDebug() << "Client IP : " << strClientIP;
	qDebug() << "Client Port : " << ui16ClientPort;
	qDebug() << "Client Name : " << strClientName;
	qDebug() << "Message : " << baData;
	qDebug() << QString(20, '*');

	m_pclsSimpleTcpServer->sendMessage(strClientIP, ui16ClientPort, "server recv : " + baData);
}