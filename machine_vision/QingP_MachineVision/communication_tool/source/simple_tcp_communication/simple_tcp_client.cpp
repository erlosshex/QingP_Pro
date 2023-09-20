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

void SimpleTcpClient::getServerInfo(QString& strServerIP, quint16& ui16ServerPort)
{
	strServerIP = getServerIP();
	ui16ServerPort = getServerPort();
}

void SimpleTcpClient::setServerIP(QString strServerIP)
{
	m_strServerIP = strServerIP;
}

void SimpleTcpClient::setServerPort(quint16 ui16ServerPort)
{
	m_ui16ServerPort = ui16ServerPort;
}

void SimpleTcpClient::setServerInfo(QString strServerIP, quint16 ui16ServerPort)
{
	setServerIP(strServerIP);
	setServerPort(ui16ServerPort);
}

void SimpleTcpClient::connectServer()
{
	connectToHost(m_strServerIP, m_ui16ServerPort);
}

void SimpleTcpClient::sendMessage(QString strMessage)
{
	QByteArray baMessage = strMessage.toUtf8();
	write(baMessage);
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



