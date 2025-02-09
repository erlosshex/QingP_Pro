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
	// �����źźͲ�
	void initConnection();

	// ����ؼ�����
	Ui::WG_HelloWorldClass ui;

	// ˢ��״̬����
	bool m_bRefreshStatus = false;

private slots:
	// ˢ�°�ť������ӵĲۺ���
	void slotRefreshMessage();
};
