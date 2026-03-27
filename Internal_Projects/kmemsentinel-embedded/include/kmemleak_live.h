#ifndef KMEMLEAK_LIVE_H
#define KMEMLEAK_LIVE_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define KMEMLEAK_DEFAULT_PATH "/sys/kernel/debug/kmemleak"
#define KMEMLEAK_BUFFER_SIZE (1024U * 1024U)

typedef struct {
    unsigned int suspected_objects;
    size_t suspected_bytes;
    unsigned int parsed_size_entries;
} kmemleak_report_t;

int kmemleak_trigger_scan(const char *kmemleak_path);
int kmemleak_read_text(const char *path, char *buffer, size_t buffer_size,
                       size_t *out_bytes);
int kmemleak_analyze_text(const char *text, kmemleak_report_t *out_report);
int kmemleak_analyze_file(const char *path, kmemleak_report_t *out_report);

#ifdef __cplusplus
}
#endif

#endif
