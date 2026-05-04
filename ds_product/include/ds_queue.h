#ifndef DS_QUEUE_H
#define DS_QUEUE_H

#include <stddef.h>
#include <stdint.h>

/* Fixed-size circular queue for int items (userspace-friendly).
 * Uses count to allow full capacity without a sentinel slot.
 */
typedef struct {
    int *buf;
    size_t capacity;
    size_t head;
    size_t tail;
    size_t count;
} ds_queue_t;

int ds_queue_init(ds_queue_t *q, int *storage, size_t capacity);
int ds_queue_is_empty(const ds_queue_t *q);
int ds_queue_is_full(const ds_queue_t *q);
int ds_queue_enqueue(ds_queue_t *q, int value);
int ds_queue_dequeue(ds_queue_t *q, int *out_value);

#endif
