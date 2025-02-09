#pragma once

#include <QWidget>
#include "ui_WG_HelloWorld.h"

class WG_HelloWorld : public QWidget
{
	Q_OBJECT

public:
	WG_HelloWorld(QWidget *parent = nullptr);
	~WG_HelloWorld();

private:
	// 连接信号和槽
	void initConnection();

	// 界面控件变量
	Ui::WG_HelloWorldClass ui;

	// 刷新状态变量
	bool m_bRefreshStatus = false;

private slots:
	// 刷新按钮点击连接的槽函数
	void slotRefreshMessage();
};
