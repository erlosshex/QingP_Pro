#pragma once

//#include <log4cplus/logger.h>
//#include <log4cplus/consoleappender.h>
//#include <log4cplus/layout.h>

#include <log4cplus/log4cplus.h>

/*
log4cplus����ʹ�ò���
1.ʵ����һ����װ��������ʵ�appender����
2.ʵ����һ����װ�������ʽ��layout����
3.��layout����󶨣�attach����appender������ʡ�Դ˲��裬�򵥲�����SimpleLayout�����󶨵�logger
4.ʵ����һ����װ����־���logger���󣬲������侲̬����getInstance()���ʵ����log4cplus::Logger::getInstance("logger_name");
5.��appender����󶨣�attach����logger����
6.����logger�����ȼ�����ʡ�Դ˲��裬�������ȼ�����־���������
*/
void standard_use();