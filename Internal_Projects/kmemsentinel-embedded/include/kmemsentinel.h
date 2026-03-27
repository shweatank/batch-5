#ifndef KMEMSENTINEL_H
#define KMEMSENTINEL_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define KMEMS_MAX_TRACKED 256U

typedef struct {
    uintptr_t ptr_value;
    size_t size;
    uint32_t alloc_tick;
    uint8_t is_freed;
    char tag[32];
} kmems_record_t;

typedef struct {
    uint32_t total_allocs;
    uint32_t total_frees;
    uint32_t active_allocs;
    uint32_t suspected_leaks;
    size_t active_bytes;
    size_t leaked_bytes;
} kmems_summary_t;

void kmems_init(void);
void kmems_tick(void);

void *kmems_alloc(size_t size, const char *tag);
void kmems_free(void *ptr);

uint32_t kmems_scan(uint32_t leak_age_threshold_ticks);
void kmems_get_summary(kmems_summary_t *out_summary);

uint32_t kmems_dump_records(kmems_record_t *out_records, uint32_t max_records);

#ifdef __cplusplus
}
#endif

#endif
