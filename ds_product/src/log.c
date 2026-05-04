#include "log.h"

#include <stdio.h>
#include <time.h>

static log_level_t g_level = LOG_LEVEL_INFO;

void log_set_level(log_level_t level)
{
    g_level = level;
}

log_level_t log_get_level(void)
{
    return g_level;
}

static const char *level_str(log_level_t level)
{
    switch (level) {
    case LOG_LEVEL_ERROR: return "ERROR";
    case LOG_LEVEL_WARN:  return "WARN";
    case LOG_LEVEL_INFO:  return "INFO";
    case LOG_LEVEL_DEBUG: return "DEBUG";
    default:              return "UNK";
    }
}

void log_vwrite(log_level_t level, const char *fmt, va_list ap)
{
    if (level > g_level) {
        return;
    }

    time_t now = time(NULL);
    struct tm tm_now;
    if (localtime_r(&now, &tm_now) != NULL) {
        char ts[32];
        if (strftime(ts, sizeof(ts), "%Y-%m-%d %H:%M:%S", &tm_now) > 0) {
            fprintf(stderr, "[%s] %s: ", ts, level_str(level));
        } else {
            fprintf(stderr, "%s: ", level_str(level));
        }
    } else {
        fprintf(stderr, "%s: ", level_str(level));
    }

    vfprintf(stderr, fmt, ap);
    fputc('\n', stderr);
}

static void log_write(log_level_t level, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    log_vwrite(level, fmt, ap);
    va_end(ap);
}

void log_error(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    log_vwrite(LOG_LEVEL_ERROR, fmt, ap);
    va_end(ap);
}

void log_warn(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    log_vwrite(LOG_LEVEL_WARN, fmt, ap);
    va_end(ap);
}

void log_info(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    log_vwrite(LOG_LEVEL_INFO, fmt, ap);
    va_end(ap);
}

void log_debug(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    log_vwrite(LOG_LEVEL_DEBUG, fmt, ap);
    va_end(ap);
}

