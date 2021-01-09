#ifndef SRC_UTIL_DEBUG_H_
#define SRC_UTIL_DEBUG_H_

#include <stdio.h>
#include <iostream>
#include <sstream>

using namespace std;

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

public:
	static int mLoggingLevel;
	static int NP_LOG_LEVEL_TRACE;
	static int NP_LOG_LEVEL_INFO;
	static int NP_LOG_LEVEL_DEBUG;
	static int NP_LOG_LEVEL_WARN;
	static int NP_LOG_LEVEL_ERROR;
};

#define RESETCOLOR 			"\033[0m"
#define LOG_RED_COLOR		"\x1b[31m"
#define LOG_GREEN_COLOR		"\x1b[32m"
#define LOG_YELLOW_COLOR	"\x1b[33m"
#define LOG_BLUE_COLOR		"\x1b[34m"
#define LOG_CYAN_COLOR		"\x1b[36m"

#define NP_LOGGER(level,fmt, EXPR...)           \
	if(Logger::IsLoggingEnabled(level))       \
	{                                             \
		std::ostringstream os;                      \
		if(level == Logger::NP_LOG_LEVEL_TRACE) 		fprintf(stderr, LOG_CYAN_COLOR "%s:%d:%s(): " fmt, __FILE__, __LINE__, __FUNCTION__, ##EXPR); \
		else if(level == Logger::NP_LOG_LEVEL_INFO) 	fprintf(stderr, LOG_GREEN_COLOR "%s:%d:%s(): " fmt, __FILE__, __LINE__, __FUNCTION__, ##EXPR); \
		else if(level == Logger::NP_LOG_LEVEL_DEBUG) 	fprintf(stderr, LOG_YELLOW_COLOR "%s:%d:%s(): " fmt, __FILE__, __LINE__, __FUNCTION__, ##EXPR); \
		else if(level == Logger::NP_LOG_LEVEL_WARN) 	fprintf(stderr, LOG_BLUE_COLOR "%s:%d:%s(): " fmt, __FILE__, __LINE__, __FUNCTION__, ##EXPR); \
		else if(level == Logger::NP_LOG_LEVEL_ERROR)	fprintf(stderr, LOG_RED_COLOR "%s:%d:%s(): " fmt, __FILE__, __LINE__, __FUNCTION__, ##EXPR); \
		fprintf(stderr, RESETCOLOR ""); \
	}                                             \
	else (void) 0;

#endif /* SRC_UTIL_DEBUG_H_ */
