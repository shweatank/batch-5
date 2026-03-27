// circular_queue.c
// Beginner-friendly Circular Queue with menu operations.
// Compile: gcc circular_queue.c -o output
// Run:     ./output

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MENU_TEXT \
    "\n==== Circular Queue Menu ====\n" \
    "1) Enqueue (insert)\n" \
    "2) Dequeue (delete)\n" \
    "3) Peek front\n" \
    "4) Display queue\n" \
    "5) Search value\n" \
    "6) Clear queue\n" \
    "0) Exit\n" \
    "Enter choice: "

typedef struct {
    int *data;
    int capacity;
    int head;   // index of front element
    int tail;   // index where next enqueue happens
    int size;   // number of elements currently in queue
} CircularQueue;

static void flush_stdin(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

static int read_int(const char *prompt, int *out) {
    char line[128];
    long v;
    char *endptr;

    if (prompt) {
        printf("%s", prompt);
        fflush(stdout);
    }

    if (!fgets(line, (int)sizeof(line), stdin)) {
        return 0;
    }

    // Remove trailing newline (if present)
    line[strcspn(line, "\n")] = '\0';

    if (line[0] == '\0') return 0;

    v = strtol(line, &endptr, 10);
    if (endptr == line || *endptr != '\0') return 0;

    if (v < INT_MIN || v > INT_MAX) return 0;

    *out = (int)v;
    return 1;
}

static int cq_init(CircularQueue *q, int capacity) {
    if (!q || capacity <= 0) return 0;

    q->data = (int *)malloc((size_t)capacity * sizeof(int));
    if (!q->data) return 0;

    q->capacity = capacity;
    q->head = 0;
    q->tail = 0;
    q->size = 0;
    return 1;
}

static void cq_free(CircularQueue *q) {
    if (!q) return;
    free(q->data);
    q->data = NULL;
    q->capacity = 0;
    q->head = q->tail = q->size = 0;
}

static int cq_is_empty(const CircularQueue *q) {
    return q && q->size == 0;
}

static int cq_is_full(const CircularQueue *q) {
    return q && q->size == q->capacity;
}

static int cq_enqueue(CircularQueue *q, int value) {
    if (!q || cq_is_full(q)) return 0;

    q->data[q->tail] = value;
    q->tail = (q->tail + 1) % q->capacity;
    q->size++;
    return 1;
}

static int cq_dequeue(CircularQueue *q, int *out_value) {
    if (!q || cq_is_empty(q)) return 0;

    if (out_value) {
        *out_value = q->data[q->head];
    }

    q->head = (q->head + 1) % q->capacity;
    q->size--;
    return 1;
}

static int cq_peek_front(const CircularQueue *q, int *out_value) {
    if (!q || cq_is_empty(q) || !out_value) return 0;

    *out_value = q->data[q->head];
    return 1;
}

static void cq_display(const CircularQueue *q) {
    if (!q) return;

    if (cq_is_empty(q)) {
        printf("Queue is empty.\n");
        return;
    }

    printf("Queue (front -> back): ");
    for (int i = 0; i < q->size; i++) {
        int idx = (q->head + i) % q->capacity;
        printf("%d", q->data[idx]);
        if (i != q->size - 1) printf(" ");
    }
    printf("\n");
}

static int cq_search(const CircularQueue *q, int value) {
    if (!q || cq_is_empty(q)) return -1;

    for (int i = 0; i < q->size; i++) {
        int idx = (q->head + i) % q->capacity;
        if (q->data[idx] == value) {
            // Return 1-based position in queue (front is position 1)
            return i + 1;
        }
    }
    return -1;
}

static void cq_clear(CircularQueue *q) {
    if (!q) return;
    q->head = 0;
    q->tail = 0;
    q->size = 0;
}

int main(void) {
    CircularQueue q;
    int capacity;

    printf("Enter queue capacity (max elements): ");
    if (!read_int(NULL, &capacity) || capacity <= 0) {
        printf("Invalid capacity. Exiting.\n");
        return 1;
    }

    if (!cq_init(&q, capacity)) {
        printf("Memory allocation failed. Exiting.\n");
        return 1;
    }

    printf("Circular Queue created with capacity %d.\n", capacity);

    for (;;) {
        int choice;

        printf(MENU_TEXT);
        if (!read_int(NULL, &choice)) {
            printf("Invalid input. Please enter a number.\n");
            // If fgets failed due to input issues, this won't always recover,
            // but it's fine for beginner usage.
            continue;
        }

        switch (choice) {
            case 1: {
                int value;
                if (!read_int("Enter value to enqueue: ", &value)) {
                    printf("Invalid value.\n");
                    break;
                }
                if (!cq_enqueue(&q, value)) {
                    printf("Queue is full. Enqueue failed.\n");
                } else {
                    printf("Enqueued %d.\n", value);
                }
                break;
            }

            case 2: {
                int value;
                if (!cq_dequeue(&q, &value)) {
                    printf("Queue is empty. Dequeue failed.\n");
                } else {
                    printf("Dequeued %d.\n", value);
                }
                break;
            }

            case 3: {
                int value;
                if (!cq_peek_front(&q, &value)) {
                    printf("Queue is empty. Nothing to peek.\n");
                } else {
                    printf("Front element: %d\n", value);
                }
                break;
            }

            case 4:
                cq_display(&q);
                break;

            case 5: {
                int value;
                if (!read_int("Enter value to search: ", &value)) {
                    printf("Invalid value.\n");
                    break;
                }
                int pos = cq_search(&q, value);
                if (pos == -1) {
                    printf("%d not found in the queue.\n", value);
                } else {
                    printf("%d found at position %d (front=1).\n", value, pos);
                }
                break;
            }

            case 6:
                cq_clear(&q);
                printf("Queue cleared.\n");
                break;

            case 0:
                cq_free(&q);
                printf("Exiting.\n");
                return 0;

            default:
                printf("Invalid choice. Please enter 0-6.\n");
                break;
        }
    }
}