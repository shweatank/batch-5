#include "ds_queue.h"

int ds_queue_init(ds_queue_t *q, int *storage, size_t capacity)
{
    if (q == NULL || storage == NULL) {
        return -1;
    }
    if (capacity == 0u) {
        return -1;
    }

    q->buf = storage;
    q->capacity = capacity;
    q->head = 0u;
    q->tail = 0u;
    q->count = 0u;
    return 0;
}

int ds_queue_is_empty(const ds_queue_t *q)
{
    return (q == NULL) ? 1 : (q->count == 0u);
}

int ds_queue_is_full(const ds_queue_t *q)
{
    return (q == NULL) ? 0 : (q->count == q->capacity);
}

int ds_queue_enqueue(ds_queue_t *q, int value)
{
    if (q == NULL || q->buf == NULL || q->capacity == 0u) {
        return -1;
    }
    if (q->count == q->capacity) {
        return -2;
    }

    q->buf[q->tail] = value;
    q->tail = (q->tail + 1u) % q->capacity;
    q->count++;
    return 0;
}

int ds_queue_dequeue(ds_queue_t *q, int *out_value)
{
    if (q == NULL || out_value == NULL || q->buf == NULL || q->capacity == 0u) {
        return -1;
    }
    if (q->count == 0u) {
        return -2;
    }

    *out_value = q->buf[q->head];
    q->head = (q->head + 1u) % q->capacity;
    q->count--;
    return 0;
}

