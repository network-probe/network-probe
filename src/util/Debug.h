#ifndef SRC_UTIL_DEBUG_H_
#define SRC_UTIL_DEBUG_H_

#include <stdio.h>

#define np_print(fmt, args...) fprintf(stderr, "[%s:%d:%s()]: " fmt, \
__FILE__, __LINE__, __FUNCTION__, ##args)

#define np_trace fprintf(stderr, "[%s:%d:%s()]\n", __FILE__, __LINE__, __FUNCTION__)

#endif /* SRC_UTIL_DEBUG_H_ */
