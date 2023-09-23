#include "SimpleTcpClientExample.h"

SimpleTcpClientExample::SimpleTcpClientExample()
{
	m_pclsSimpleTcpClient = new SimpleTcpClient(0, "TestClient");

	connect(m_pclsSimpleTcpClient, &SimpleTcpClient::signal_clientReadData, this, &SimpleTcpClientExample::slot_showRecvMessage);
}

SimpleTcpClientExample::~SimpleTcpClientExample()
{
	if (nullptr != m_pclsSimpleTcpClient)
	{
		delete m_pclsSimpleTcpClient;
		m_pclsSimpleTcpClient = nullptr;
	}
}

void SimpleTcpClientExample::setServerInfo(QString strServerInfo, quint16 ui16ServerPort)
{
	if (nullptr == m_pclsSimpleTcpClient)
	{
		return;
	}

	m_pclsSimpleTcpClient->setServerInfo(strServerInfo, ui16ServerPort);

	m_pclsSimpleTcpClient->connectServer();

	QThread::msleep(2000);

	m_pclsSimpleTcpClient->sendMessage("This is Test One");

}

void SimpleTcpClientExample::slot_showRecvMessage(uint uiClientID, QString strIP, uint uiPort, QByteArray baData)
{
	qDebug() << QString(20, '*');
	qDebug() << "Client ID : " << uiClientID;
	qDebug() << "IP : " << strIP;
	qDebug() << "Port : " << uiPort;
	qDebug() << "Message : " << baData;
	qDebug() << QString(20, '*');
}
