#include "Debug.h"

#if 0
int Logger::NP_LOG_LEVEL_TRACE = 0;
int Logger::NP_LOG_LEVEL_INFO = 1;
int Logger::NP_LOG_LEVEL_DEBUG = 2;
int Logger::NP_LOG_LEVEL_WARN = 3;
int Logger::NP_LOG_LEVEL_ERROR = 4;

int Logger::mLoggingLevel = Logger::NP_LOG_LEVEL_DEBUG;

bool Logger::IsLoggingEnabled(int level)
{
	if(level >= mLoggingLevel)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Logger::LogMessage(const char *str)
{
//	printf("%s", str);
}
#endif
