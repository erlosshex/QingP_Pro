#include "simple_tcp_server.h"

SimpleTcpServer::SimpleTcpServer(uint uiServerID, QString strServerName, QObject* parent) : QTcpServer(parent)
{
	m_uiServerID = uiServerID;
	m_strServerName = strServerName;
}

SimpleTcpServer::~SimpleTcpServer()
{
	closeServer();
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

void SimpleTcpServer::setServerPort(quint16 ui16Port)
{
	m_ui16ServerPort = ui16Port;
}

quint16 SimpleTcpServer::getServerPort()
{
	return m_ui16ServerPort;
}

QList<SimpleTcpServer::SimpleTcpClientInfo*> SimpleTcpServer::getClientList()
{
	return m_lstpstClientInfo;
}

void SimpleTcpServer::openServer()
{
	listen(QHostAddress::Any, m_ui16ServerPort);

	connect(this, &SimpleTcpServer::newConnection, this, &SimpleTcpServer::slot_newConnection);
}

void SimpleTcpServer::closeServer()
{
	for (int iIdx = 0; iIdx < m_lstpstClientInfo.size(); ++iIdx)
	{
		// 不知道这里不释放内存会不会泄露，但这里的内存不是手动申请的，是通过Server连接自动获取的
		//delete m_lstpstClientInfo[iIdx]->pTcpSocket;
		//m_lstpstClientInfo[iIdx]->pTcpSocket = nullptr;

		delete m_lstpstClientInfo[iIdx];
		m_lstpstClientInfo[iIdx] = nullptr;
	}
	m_lstpstClientInfo.clear();
	m_lstpstClientInfo.squeeze();
}

void SimpleTcpServer::sendMessage(QString strClientIP, quint16 ui16ClientPort, QString strMessage)
{
	SimpleTcpClientInfo* pstClientInfo = nullptr;
	for (int iIdx = 0; iIdx < m_lstpstClientInfo.size(); ++iIdx)
	{
		if (nullptr == m_lstpstClientInfo[iIdx])
		{
			continue;
		}

		if (strClientIP == m_lstpstClientInfo[iIdx]->strClientIP &&
			ui16ClientPort == m_lstpstClientInfo[iIdx]->ui16ClientPort)
		{
			pstClientInfo = m_lstpstClientInfo[iIdx];
			break;
		}
	}

	if (nullptr == pstClientInfo)
	{
		return;
	}

	QByteArray baData = strMessage.toUtf8();
	pstClientInfo->pTcpSocket->write(baData);
}

void SimpleTcpServer::slot_newConnection()
{
	m_mtxNewConnection.lock();

	SimpleTcpClientInfo* pstClientInfo = new SimpleTcpClientInfo;

	pstClientInfo->pTcpSocket = this->nextPendingConnection();
	pstClientInfo->strClientIP = pstClientInfo->pTcpSocket->peerAddress().toString();
	pstClientInfo->ui16ClientPort = pstClientInfo->pTcpSocket->peerPort();
	pstClientInfo->strClientName = pstClientInfo->pTcpSocket->peerName();
	pstClientInfo->uiClientID = m_lstpstClientInfo.size();

	m_lstpstClientInfo.append(pstClientInfo);
	m_iClientCount = m_lstpstClientInfo.size();

	connect(pstClientInfo->pTcpSocket, &QTcpSocket::readyRead, this, [=]() {
		QByteArray baData = pstClientInfo->pTcpSocket->readAll();
		emit signal_readyReadFromOneClient(
			pstClientInfo->uiClientID, \
			pstClientInfo->strClientIP, \
			pstClientInfo->ui16ClientPort, \
			pstClientInfo->strClientName, \
			baData
		);
		});

	m_mtxNewConnection.unlock();
}
