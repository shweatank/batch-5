#include "ds_stack.h"

int ds_stack_init(ds_stack_t *s, int *storage, size_t capacity)
{
    if (s == NULL || storage == NULL) {
        return -1;
    }
    if (capacity == 0u) {
        return -1;
    }
    s->buf = storage;
    s->capacity = capacity;
    s->top = 0u;
    return 0;
}

int ds_stack_is_empty(const ds_stack_t *s)
{
    return (s == NULL) ? 1 : (s->top == 0u);
}

int ds_stack_is_full(const ds_stack_t *s)
{
    return (s == NULL) ? 0 : (s->top == s->capacity);
}

int ds_stack_push(ds_stack_t *s, int value)
{
    if (s == NULL || s->buf == NULL || s->capacity == 0u) {
        return -1;
    }
    if (s->top == s->capacity) {
        return -2;
    }
    s->buf[s->top] = value;
    s->top++;
    return 0;
}

int ds_stack_pop(ds_stack_t *s, int *out_value)
{
    if (s == NULL || out_value == NULL || s->buf == NULL || s->capacity == 0u) {
        return -1;
    }
    if (s->top == 0u) {
        return -2;
    }
    s->top--;
    *out_value = s->buf[s->top];
    return 0;
}

