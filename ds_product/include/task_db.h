#ifndef TASK_DB_H
#define TASK_DB_H

#include <stddef.h>
#include <stdint.h>

typedef enum {
    TASK_STATUS_TODO = 0,
    TASK_STATUS_DONE = 1
} task_status_t;

typedef struct {
    int id;
    task_status_t status;
    char title[64];
} task_t;

typedef struct task_db task_db_t;

/* Lifecycle */
int task_db_init(task_db_t **out_db);
void task_db_deinit(task_db_t *db);

/* CRUD */
int task_db_add(task_db_t *db, const char *title, int *out_id);
int task_db_mark_done(task_db_t *db, int id);
int task_db_delete(task_db_t *db, int id);
int task_db_undo_last_delete(task_db_t *db);

/* Query */
size_t task_db_count(const task_db_t *db);
int task_db_get(const task_db_t *db, int id, task_t *out_task);
void task_db_list(const task_db_t *db);

/* Persistence (simple text format) */
int task_db_load(task_db_t *db, const char *path);
int task_db_save(const task_db_t *db, const char *path);

#endif
