#include "task_db.h"

#include "ds_stack.h"
#include "log.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Simple userspace "DB":
 * - dynamic array of tasks
 * - stack of deleted task indices for undo (stores task ID)
 */

struct task_db {
    task_t *tasks;
    size_t count;
    size_t cap;
    int next_id;

    ds_stack_t undo_ids;
    int undo_storage[64];
};

static int ensure_cap(task_db_t *db, size_t want)
{
    if (db->cap >= want) {
        return 0;
    }

    size_t new_cap = (db->cap == 0u) ? 16u : (db->cap * 2u);
    while (new_cap < want) {
        new_cap *= 2u;
    }

    task_t *p = (task_t *)realloc(db->tasks, new_cap * sizeof(task_t));
    if (p == NULL) {
        return -1;
    }
    db->tasks = p;
    db->cap = new_cap;
    return 0;
}

static ssize_t find_index_by_id(const task_db_t *db, int id)
{
    for (size_t i = 0u; i < db->count; i++) {
        if (db->tasks[i].id == id) {
            return (ssize_t)i;
        }
    }
    return -1;
}

int task_db_init(task_db_t **out_db)
{
    if (out_db == NULL) {
        return -1;
    }

    task_db_t *db = (task_db_t *)calloc(1, sizeof(*db));
    if (db == NULL) {
        return -1;
    }

    db->tasks = NULL;
    db->count = 0u;
    db->cap = 0u;
    db->next_id = 1;

    if (ds_stack_init(&db->undo_ids, db->undo_storage, (sizeof(db->undo_storage) / sizeof(db->undo_storage[0]))) != 0) {
        free(db);
        return -1;
    }

    *out_db = db;
    return 0;
}

void task_db_deinit(task_db_t *db)
{
    if (db == NULL) {
        return;
    }
    free(db->tasks);
    free(db);
}

int task_db_add(task_db_t *db, const char *title, int *out_id)
{
    if (db == NULL || title == NULL) {
        return -1;
    }
    if (title[0] == '\0') {
        return -2;
    }

    if (ensure_cap(db, db->count + 1u) != 0) {
        return -3;
    }

    task_t *t = &db->tasks[db->count];
    t->id = db->next_id++;
    t->status = TASK_STATUS_TODO;
    (void)snprintf(t->title, sizeof(t->title), "%s", title);

    db->count++;

    if (out_id != NULL) {
        *out_id = t->id;
    }
    return 0;
}

int task_db_get(const task_db_t *db, int id, task_t *out_task)
{
    if (db == NULL || out_task == NULL) {
        return -1;
    }
    ssize_t idx = find_index_by_id(db, id);
    if (idx < 0) {
        return -2;
    }
    *out_task = db->tasks[(size_t)idx];
    return 0;
}

int task_db_mark_done(task_db_t *db, int id)
{
    if (db == NULL) {
        return -1;
    }
    ssize_t idx = find_index_by_id(db, id);
    if (idx < 0) {
        return -2;
    }
    db->tasks[(size_t)idx].status = TASK_STATUS_DONE;
    return 0;
}

int task_db_delete(task_db_t *db, int id)
{
    if (db == NULL) {
        return -1;
    }
    ssize_t idx = find_index_by_id(db, id);
    if (idx < 0) {
        return -2;
    }

    /* Push ID for undo (best-effort) */
    (void)ds_stack_push(&db->undo_ids, id);

    size_t i = (size_t)idx;
    /* Stable delete: shift left */
    for (; i + 1u < db->count; i++) {
        db->tasks[i] = db->tasks[i + 1u];
    }
    db->count--;
    return 0;
}

int task_db_undo_last_delete(task_db_t *db)
{
    if (db == NULL) {
        return -1;
    }

    int id = 0;
    if (ds_stack_pop(&db->undo_ids, &id) != 0) {
        return -2;
    }

    /* Can't reconstruct deleted title without storing full task snapshot.
     * For a minimal product: restore a placeholder task with same id.
     */
    if (ensure_cap(db, db->count + 1u) != 0) {
        return -3;
    }

    task_t *t = &db->tasks[db->count];
    t->id = id;
    t->status = TASK_STATUS_TODO;
    (void)snprintf(t->title, sizeof(t->title), "RESTORED task id=%d", id);
    db->count++;

    if (id >= db->next_id) {
        db->next_id = id + 1;
    }

    return 0;
}

size_t task_db_count(const task_db_t *db)
{
    return (db == NULL) ? 0u : db->count;
}

static void print_task(const task_t *t)
{
    const char *st = (t->status == TASK_STATUS_DONE) ? "DONE" : "TODO";
    printf("%d [%s] %s\n", t->id, st, t->title);
}

void task_db_list(const task_db_t *db)
{
    if (db == NULL) {
        return;
    }

    if (db->count == 0u) {
        puts("(no tasks)");
        return;
    }

    for (size_t i = 0u; i < db->count; i++) {
        print_task(&db->tasks[i]);
    }
}

int task_db_load(task_db_t *db, const char *path)
{
    if (db == NULL || path == NULL) {
        return -1;
    }

    FILE *fp = fopen(path, "r");
    if (fp == NULL) {
        /* Missing file is not fatal */
        if (errno == ENOENT) {
            return 0;
        }
        return -2;
    }

    char line[256];
    while (fgets(line, sizeof(line), fp) != NULL) {
        /* Format: id|status|title\n */
        char *p = strchr(line, '\n');
        if (p) *p = '\0';

        char *id_s = strtok(line, "|");
        char *st_s = strtok(NULL, "|");
        char *ti_s = strtok(NULL, "|");
        if (id_s == NULL || st_s == NULL || ti_s == NULL) {
            continue;
        }

        errno = 0;
        long id_l = strtol(id_s, NULL, 10);
        long st_l = strtol(st_s, NULL, 10);
        if (errno != 0 || id_l <= 0) {
            continue;
        }

        int id = (int)id_l;
        task_status_t st = (st_l == 1) ? TASK_STATUS_DONE : TASK_STATUS_TODO;

        if (ensure_cap(db, db->count + 1u) != 0) {
            fclose(fp);
            return -3;
        }
        task_t *t = &db->tasks[db->count++];
        t->id = id;
        t->status = st;
        (void)snprintf(t->title, sizeof(t->title), "%s", ti_s);

        if (id >= db->next_id) {
            db->next_id = id + 1;
        }
    }

    fclose(fp);
    log_info("Loaded %zu tasks from %s", db->count, path);
    return 0;
}

int task_db_save(const task_db_t *db, const char *path)
{
    if (db == NULL || path == NULL) {
        return -1;
    }

    FILE *fp = fopen(path, "w");
    if (fp == NULL) {
        return -2;
    }

    for (size_t i = 0u; i < db->count; i++) {
        const task_t *t = &db->tasks[i];
        /* id|status|title */
        fprintf(fp, "%d|%d|%s\n", t->id, (t->status == TASK_STATUS_DONE) ? 1 : 0, t->title);
    }

    fclose(fp);
    log_info("Saved %zu tasks to %s", db->count, path);
    return 0;
}

