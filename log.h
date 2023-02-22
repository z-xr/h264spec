#ifndef LOG_H
#define LOG_H
#include <stdio.h>

static int log_level = 1;

enum LOG_LEVEL
{
    LOG_LEVEL_ERROR = 0,
    LOG_LEVEL_WARN = 1,
    LOG_LEVEL_INFO = 2,
    LOG_LEVEL_DEBUG = 3,
    LOG_LEVEL_ALL = 4,
};
	
#define LOGE(fmt,args...) printf(fmt, ##args);


#define LOGW(fmt,args...)	\
do {							\
	if (log_level >= LOG_LEVEL_WARN) {	\
		printf(fmt, ##args);	\
	}	\
} while(0)

#define LOGI(fmt,args...)	\
do {							\
	if (log_level >= LOG_LEVEL_INFO) {	\
		printf(fmt, ##args);	\
	}	\
} while(0)

#define LOGD(fmt,args...)	\
do {							\
	if (log_level >= LOG_LEVEL_DEBUG) {	\
		printf(fmt, ##args);	\
	}	\
} while(0)

#define LOGA(fmt,args...)	\
do {							\
	if (log_level >= LOG_LEVEL_ALL) { \
		printf(fmt, ##args);	\
	}	\
} while(0)

#endif
