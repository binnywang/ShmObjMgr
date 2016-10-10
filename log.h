#ifndef LOG_H_
#define LOG_H_

#include <stdio.h>

enum LogLevel {
	LOG_LEVEL_DEBUG,
	LOG_LEVEL_ERROR,
};

static const LogLevel cur_log_level = LOG_LEVEL_ERROR;

#define LOG(log_level, format, ...) \
	do { \
		if (log_level >= cur_log_level) { \
			printf(format, ##__VA_ARGS__); \
		} \
	} while(0)

#define LOG_ERROR(format, ...)  LOG(LOG_LEVEL_ERROR, format, ##__VA_ARGS__)
#define LOG_DEBUG(format, ...)  LOG(LOG_LEVEL_DEBUG, format, ##__VA_ARGS__)

#endif //LOG_H_
