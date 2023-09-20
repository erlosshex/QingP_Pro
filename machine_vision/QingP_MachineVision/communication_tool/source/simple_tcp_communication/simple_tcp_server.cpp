#include "simple_tcp_server.h"

SimpleTcpServer::SimpleTcpServer(uint uiServerID, QString strServerName, QObject* parent) : QTcpServer(parent)
{
	m_uiServerID = uiServerID;
	m_strServerName = strServerName;
}

SimpleTcpServer::~SimpleTcpServer()
{

}

