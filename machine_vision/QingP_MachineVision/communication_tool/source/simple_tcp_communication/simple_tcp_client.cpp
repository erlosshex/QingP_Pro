#include "simple_tcp_client.h"


SimpleTcpClient::SimpleTcpClient(uint uiClientID, QString strClientName, QObject* parent) : QTcpSocket(parent)
{
	m_uiClientID = uiClientID;
	m_strClientName = strClientName;

	connect(this, &SimpleTcpClient::readyRead, this, &SimpleTcpClient::slot_clientReadData);
	connect(this, &SimpleTcpClient::disconnected, this, &SimpleTcpClient::slot_disconnect);

	connect(this, &SimpleTcpClient::disconnected, this, &SimpleTcpClient::deleteLater);
}

SimpleTcpClient::~SimpleTcpClient()
{

}

uint SimpleTcpClient::getClientID()
{
	return m_uiClientID;
}

QString SimpleTcpClient::getClientName()
{
	return m_strClientName;
}

QString SimpleTcpClient::getServerIP()
{
	return m_strServerIP;
}

quint16 SimpleTcpClient::getServerPort()
{
	return m_ui16ServerPort;
}

void SimpleTcpClient::slot_clientReadData()
{
	QThread::msleep(100);

	QByteArray baData = readAll();

	emit signal_clientReadData(
		this->m_uiClientID, \
		this->peerAddress().toString(), \
		this->peerPort(), \
		baData
	);
}

void SimpleTcpClient::slot_disconnect()
{
	emit signal_clientDisconnect(
		this->m_uiClientID, \
		this->peerAddress().toString(), \
		this->peerPort()
	);
}



