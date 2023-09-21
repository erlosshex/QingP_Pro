#include "simple_tcp_server.h"

SimpleTcpServer::SimpleTcpServer(uint uiServerID, QString strServerName, QObject* parent) : QTcpServer(parent)
{
	m_uiServerID = uiServerID;
	m_strServerName = strServerName;
}

SimpleTcpServer::~SimpleTcpServer()
{

}

QString SimpleTcpServer::getServerName()
{
	return m_strServerName;
}

uint SimpleTcpServer::getServerID()
{
	return m_uiServerID;
}

void SimpleTcpServer::getServerInfo(QString& strServerName, uint& uiServerID)
{
	strServerName = getServerName();
	uiServerID = getServerID();
}

QList<SimpleTcpServer::SimpleTcpClientInfo> SimpleTcpServer::getClientList()
{
	return m_lstpstClientInfo;
}

void SimpleTcpServer::openServer()
{
	listen(QHostAddress::Any, ui16ServerPort);
}

void SimpleTcpServer::closeServer()
{

}

