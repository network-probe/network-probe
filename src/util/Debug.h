#ifndef SRC_UTIL_DEBUG_H_
#define SRC_UTIL_DEBUG_H_

#include <stdio.h>
#include <iostream>
#include <sstream>

using namespace std;

#define np_print(fmt, args...) fprintf(stderr, "[%s:%s:%d]: " fmt, \
__FILE__, __FUNCTION__, __LINE__, ##args)

#define np_trace fprintf(stderr, "[%s:%d:%s()]\n", __FILE__, __LINE__, __FUNCTION__)

#if 0
class Logger
{
public:
	Logger();
	virtual ~Logger();

	static bool IsLoggingEnabled(int level);
	static void SetLogging(int logging_level)
	{
		mLoggingLevel = logging_level;
	}
	static void LogMessage(const char *str);

public:
	static int mLoggingLevel;
	static int NP_LOG_LEVEL_TRACE;
	static int NP_LOG_LEVEL_INFO;
	static int NP_LOG_LEVEL_DEBUG;
	static int NP_LOG_LEVEL_WARN;
	static int NP_LOG_LEVEL_ERROR;
};

#define NP_LOGGER_INTERNAL(level,fmt, EXPR...)           \
	if(Logger::IsLoggingEnabled(level))       \
	{                                             \
		std::ostringstream os;                      \
		fprintf(stderr, "[%s:%d:%s()]: " fmt, __FILE__, __LINE__, __FUNCTION__, ##EXPR) \
	}                                             \
	else (void) 0

#define NP_LOG(THELEVEL,fmt, EXPR)                 \
		NP_LOGGER_INTERNAL(Logger::NP_LOG_LEVEL_ ## THELEVEL,fmt,EXPR)

#define NP_LOG_TRACE(fmt, EXPR...)	NP_LOG(TRACE, fmt, EXPR...)
#define NP_LOG_INFO(fmt, EXPR...)	NP_LOG(INFO, fmt, EXPR...)
#define NP_LOG_DEBUG(fmt, EXPR...)	NP_LOG(DEBUG, fmt, EXPR...)
#define NP_LOG_WARN(fmt, EXPR...)	NP_LOG(WARN, fmt, EXPR...)
#define NP_LOG_ERROR(fmt, EXPR...)	NP_LOG(ERROR, fmt, EXPR...)
#endif

#endif /* SRC_UTIL_DEBUG_H_ */
