#ifndef DS_LIST_H
#define DS_LIST_H

#include <stddef.h>

typedef struct ds_list_node {
    int value;
    struct ds_list_node *next;
} ds_list_node_t;

typedef struct {
    ds_list_node_t *head;
    size_t size;
} ds_list_t;

void ds_list_init(ds_list_t *list);
int ds_list_push_front(ds_list_t *list, ds_list_node_t *node, int value);
int ds_list_remove_first_value(ds_list_t *list, int value, ds_list_node_t **out_node);
ds_list_node_t *ds_list_find(const ds_list_t *list, int value);
void ds_list_foreach(const ds_list_t *list, void (*fn)(const ds_list_node_t *node, void *ctx), void *ctx);

#endif
