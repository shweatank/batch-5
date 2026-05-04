#include "input.h"

#include "log.h"

#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int input_read_line(const char *prompt, char *buf, size_t buf_len)
{
    if (buf == NULL || buf_len == 0u) {
        return -1;
    }

    if (prompt != NULL) {
        fputs(prompt, stdout);
        fflush(stdout);
    }

    if (fgets(buf, (int)buf_len, stdin) == NULL) {
        /* EOF or error */
        return -2;
    }

    size_t n = strnlen(buf, buf_len);
    if (n > 0u && buf[n - 1u] == '\n') {
        buf[n - 1u] = '\0';
        return 0;
    }

    /* Line longer than buffer: flush remaining characters */
    int ch;
    do {
        ch = fgetc(stdin);
    } while (ch != '\n' && ch != EOF);

    log_warn("Input truncated (max %zu chars)", buf_len - 1u);
    return 0;
}

static int is_all_space(const char *s)
{
    if (s == NULL) {
        return 1;
    }
    while (*s != '\0') {
        if (!isspace((unsigned char)*s)) {
            return 0;
        }
        s++;
    }
    return 1;
}

int input_read_int(const char *prompt, int *out_value, int min_value, int max_value)
{
    if (out_value == NULL) {
        return -1;
    }
    if (min_value > max_value) {
        return -1;
    }

    char line[64];
    for (;;) {
        int rc = input_read_line(prompt, line, sizeof(line));
        if (rc != 0) {
            return rc;
        }
        if (is_all_space(line)) {
            log_warn("Please enter a number.");
            continue;
        }

        errno = 0;
        char *end = NULL;
        long v = strtol(line, &end, 10);
        if (errno != 0) {
            log_warn("Invalid number (errno=%d).", errno);
            continue;
        }
        if (end == line) {
            log_warn("Invalid number.");
            continue;
        }
        while (*end != '\0' && isspace((unsigned char)*end)) {
            end++;
        }
        if (*end != '\0') {
            log_warn("Trailing characters not allowed.");
            continue;
        }
        if (v < (long)min_value || v > (long)max_value) {
            log_warn("Out of range [%d..%d].", min_value, max_value);
            continue;
        }
        if (v < INT_MIN || v > INT_MAX) {
            log_warn("Out of int range.");
            continue;
        }

        *out_value = (int)v;
        return 0;
    }
}

