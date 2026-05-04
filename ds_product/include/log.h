#ifndef LOG_H
#define LOG_H

#include <stdarg.h>

typedef enum {
    LOG_LEVEL_ERROR = 0,
    LOG_LEVEL_WARN  = 1,
    LOG_LEVEL_INFO  = 2,
    LOG_LEVEL_DEBUG = 3
} log_level_t;

void log_set_level(log_level_t level);
log_level_t log_get_level(void);

void log_error(const char *fmt, ...);
void log_warn(const char *fmt, ...);
void log_info(const char *fmt, ...);
void log_debug(const char *fmt, ...);

/* Internal (exposed for tests if needed) */
void log_vwrite(log_level_t level, const char *fmt, va_list ap);

#endif
