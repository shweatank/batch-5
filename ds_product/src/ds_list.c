#include "ds_list.h"

void ds_list_init(ds_list_t *list)
{
    if (list == NULL) {
        return;
    }
    list->head = NULL;
    list->size = 0u;
}

int ds_list_push_front(ds_list_t *list, ds_list_node_t *node, int value)
{
    if (list == NULL || node == NULL) {
        return -1;
    }
    node->value = value;
    node->next = list->head;
    list->head = node;
    list->size++;
    return 0;
}

ds_list_node_t *ds_list_find(const ds_list_t *list, int value)
{
    if (list == NULL) {
        return NULL;
    }
    ds_list_node_t *cur = list->head;
    while (cur != NULL) {
        if (cur->value == value) {
            return cur;
        }
        cur = cur->next;
    }
    return NULL;
}

int ds_list_remove_first_value(ds_list_t *list, int value, ds_list_node_t **out_node)
{
    if (list == NULL || out_node == NULL) {
        return -1;
    }

    ds_list_node_t *prev = NULL;
    ds_list_node_t *cur = list->head;
    while (cur != NULL) {
        if (cur->value == value) {
            if (prev == NULL) {
                list->head = cur->next;
            } else {
                prev->next = cur->next;
            }
            cur->next = NULL;
            list->size--;
            *out_node = cur;
            return 0;
        }
        prev = cur;
        cur = cur->next;
    }

    return -2;
}

void ds_list_foreach(const ds_list_t *list, void (*fn)(const ds_list_node_t *node, void *ctx), void *ctx)
{
    if (list == NULL || fn == NULL) {
        return;
    }

    const ds_list_node_t *cur = list->head;
    while (cur != NULL) {
        fn(cur, ctx);
        cur = cur->next;
    }
}

