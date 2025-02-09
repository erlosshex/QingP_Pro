#include "WG_HelloWorld.h"

WG_HelloWorld::WG_HelloWorld(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	// ��ʼ��ʱ���������е��ź����
	initConnection();
}

WG_HelloWorld::~WG_HelloWorld()
{

}

void WG_HelloWorld::initConnection()
{
	// ˢ�°�ť�������
	connect(ui.BTN_Refresh, &QPushButton::clicked, this, &WG_HelloWorld::slotRefreshMessage);

	return;
}

void WG_HelloWorld::slotRefreshMessage()
{
	m_bRefreshStatus = !m_bRefreshStatus;

	if (nullptr != ui.LE_Message)
	{
		QString strMessage = "";
		if (true == m_bRefreshStatus)
		{
			strMessage = "Hello,World!";
		}
		else
		{
			strMessage = "Welcome!";
		}

		ui.LE_Message->setText(strMessage);
	}

	return;
}

