#include "standard_use.h"

using namespace log4cplus;
using namespace log4cplus::helpers;

void standard_use()
{
	// step 1 : instantiate an appender object
	SharedObjectPtr<Appender> spAppend(new ConsoleAppender());
	spAppend->setName(LOG4CPLUS_C_STR_TO_TSTRING("append for test"));

	// step 2 : instantiate a layout object
	tstring strPattern = LOG4CPLUS_C_STR_TO_TSTRING("%d{%m/%d/%y  %H:%M:%S} - %m [%l] %n");
	std::unique_ptr<Layout> upLayout(new PatternLayout(strPattern));

	// step 3 : attach the layout object to the appender
	spAppend->setLayout(std::move(upLayout));

	// step 4 : instantiate a logger object
	Logger logLogger = Logger::getInstance(LOG4CPLUS_C_STR_TO_TSTRING("test"));

	// step 5 : attach the appender object to the logger
	logLogger.addAppender(spAppend);

	// step 6 : set a priority for the logger
	logLogger.setLogLevel(ALL_LOG_LEVEL);

	// log activity
	LOG4CPLUS_DEBUG(logLogger, "this is the first log message ...");
	std::this_thread::sleep_for(std::chrono::seconds(2));
	LOG4CPLUS_WARN(logLogger, "this is the second log message ...");

}