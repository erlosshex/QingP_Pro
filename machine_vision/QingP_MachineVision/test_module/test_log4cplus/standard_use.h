#pragma once

//#include <log4cplus/logger.h>
//#include <log4cplus/consoleappender.h>
//#include <log4cplus/layout.h>

#include <log4cplus/log4cplus.h>

/*
log4cplus基本使用步骤
1.实例化一个封装了输出介质的appender对象
2.实例化一个封装了输出格式的layout对象
3.将layout对象绑定（attach）到appender对象；如省略此步骤，简单布局器SimpleLayout对象会绑定到logger
4.实例化一个封装了日志输出logger对象，并调用其静态函数getInstance()获得实例，log4cplus::Logger::getInstance("logger_name");
5.将appender对象绑定（attach）到logger对象
6.设置logger的优先级，如省略此步骤，各种优先级的日志都将被输出
*/
void standard_use();