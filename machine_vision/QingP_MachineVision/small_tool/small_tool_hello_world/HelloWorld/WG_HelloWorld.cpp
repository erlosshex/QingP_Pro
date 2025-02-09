#include "WG_HelloWorld.h"

WG_HelloWorld::WG_HelloWorld(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	// 初始化时候连接所有的信号与槽
	initConnection();
}

WG_HelloWorld::~WG_HelloWorld()
{

}

void WG_HelloWorld::initConnection()
{
	// 刷新按钮点击动作
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

