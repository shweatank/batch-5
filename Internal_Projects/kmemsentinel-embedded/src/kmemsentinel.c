#include "kmemsentinel.h"

#include <stdlib.h>
#include <string.h>

typedef struct {
    void *ptr;
    size_t size;
    uint32_t alloc_tick;
    uint8_t used;
    uint8_t is_freed;
    char tag[32];
} tracker_slot_t;

static tracker_slot_t g_slots[KMEMS_MAX_TRACKED];
static uint32_t g_tick;
static kmems_summary_t g_summary;

void kmems_init(void) {
    memset(g_slots, 0, sizeof(g_slots));
    memset(&g_summary, 0, sizeof(g_summary));
    g_tick = 0U;
}

void kmems_tick(void) { g_tick++; }

static uint32_t find_slot_by_ptr(void *ptr) {
    uint32_t i;
    for (i = 0U; i < KMEMS_MAX_TRACKED; ++i) {
        if (g_slots[i].used && g_slots[i].ptr == ptr) {
            return i;
        }
    }
    return KMEMS_MAX_TRACKED;
}

static uint32_t find_free_slot(void) {
    uint32_t i;
    for (i = 0U; i < KMEMS_MAX_TRACKED; ++i) {
        if (!g_slots[i].used) {
            return i;
        }
    }
    return KMEMS_MAX_TRACKED;
}

void *kmems_alloc(size_t size, const char *tag) {
    void *ptr;
    uint32_t slot_index;
    tracker_slot_t *slot;

    if (size == 0U) {
        return NULL;
    }

    ptr = malloc(size);
    if (ptr == NULL) {
        return NULL;
    }

    slot_index = find_free_slot();
    if (slot_index == KMEMS_MAX_TRACKED) {
        free(ptr);
        return NULL;
    }

    slot = &g_slots[slot_index];
    slot->ptr = ptr;
    slot->size = size;
    slot->alloc_tick = g_tick;
    slot->used = 1U;
    slot->is_freed = 0U;

    if (tag != NULL) {
        strncpy(slot->tag, tag, sizeof(slot->tag) - 1U);
        slot->tag[sizeof(slot->tag) - 1U] = '\0';
    } else {
        slot->tag[0] = '\0';
    }

    g_summary.total_allocs++;
    g_summary.active_allocs++;
    g_summary.active_bytes += size;
    return ptr;
}

void kmems_free(void *ptr) {
    uint32_t slot_index;
    tracker_slot_t *slot;

    if (ptr == NULL) {
        return;
    }

    slot_index = find_slot_by_ptr(ptr);
    if (slot_index == KMEMS_MAX_TRACKED) {
        return;
    }

    slot = &g_slots[slot_index];
    if (slot->is_freed) {
        return;
    }

    free(ptr);
    slot->is_freed = 1U;
    slot->used = 0U;

    g_summary.total_frees++;
    if (g_summary.active_allocs > 0U) {
        g_summary.active_allocs--;
    }
    if (g_summary.active_bytes >= slot->size) {
        g_summary.active_bytes -= slot->size;
    } else {
        g_summary.active_bytes = 0U;
    }
}

uint32_t kmems_scan(uint32_t leak_age_threshold_ticks) {
    uint32_t i;
    uint32_t suspected = 0U;
    size_t leaked_bytes = 0U;

    for (i = 0U; i < KMEMS_MAX_TRACKED; ++i) {
        if (!g_slots[i].used || g_slots[i].is_freed) {
            continue;
        }

        if ((g_tick - g_slots[i].alloc_tick) >= leak_age_threshold_ticks) {
            suspected++;
            leaked_bytes += g_slots[i].size;
        }
    }

    g_summary.suspected_leaks = suspected;
    g_summary.leaked_bytes = leaked_bytes;
    return suspected;
}

void kmems_get_summary(kmems_summary_t *out_summary) {
    if (out_summary == NULL) {
        return;
    }
    *out_summary = g_summary;
}

uint32_t kmems_dump_records(kmems_record_t *out_records, uint32_t max_records) {
    uint32_t i;
    uint32_t out_index = 0U;

    if (out_records == NULL || max_records == 0U) {
        return 0U;
    }

    for (i = 0U; i < KMEMS_MAX_TRACKED && out_index < max_records; ++i) {
        if (!g_slots[i].used && !g_slots[i].is_freed) {
            continue;
        }

        out_records[out_index].ptr_value = (uintptr_t)g_slots[i].ptr;
        out_records[out_index].size = g_slots[i].size;
        out_records[out_index].alloc_tick = g_slots[i].alloc_tick;
        out_records[out_index].is_freed = g_slots[i].is_freed;
        strncpy(out_records[out_index].tag, g_slots[i].tag,
                sizeof(out_records[out_index].tag) - 1U);
        out_records[out_index].tag[sizeof(out_records[out_index].tag) - 1U] = '\0';
        out_index++;
    }

    return out_index;
}
