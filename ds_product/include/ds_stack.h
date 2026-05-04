#ifndef DS_STACK_H
#define DS_STACK_H

#include <stddef.h>

/* Fixed-size stack for int items. */
typedef struct {
    int *buf;
    size_t capacity;
    size_t top; /* number of elements */
} ds_stack_t;

int ds_stack_init(ds_stack_t *s, int *storage, size_t capacity);
int ds_stack_is_empty(const ds_stack_t *s);
int ds_stack_is_full(const ds_stack_t *s);
int ds_stack_push(ds_stack_t *s, int value);
int ds_stack_pop(ds_stack_t *s, int *out_value);

#endif
