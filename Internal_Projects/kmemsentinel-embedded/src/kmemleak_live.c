#include "kmemleak_live.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int parse_size_from_header(const char *line, size_t *out_size) {
    const char *size_key = "(size ";
    const char *size_pos = strstr(line, size_key);
    char *end_ptr = NULL;
    unsigned long parsed_size;

    if (size_pos == NULL || out_size == NULL) {
        return 0;
    }

    size_pos += strlen(size_key);
    errno = 0;
    parsed_size = strtoul(size_pos, &end_ptr, 10);
    if (errno != 0 || end_ptr == size_pos) {
        return 0;
    }

    *out_size = (size_t)parsed_size;
    return 1;
}

int kmemleak_trigger_scan(const char *kmemleak_path) {
    FILE *file;
    const char *path = kmemleak_path != NULL ? kmemleak_path : KMEMLEAK_DEFAULT_PATH;

    file = fopen(path, "w");
    if (file == NULL) {
        return -1;
    }

    if (fputs("scan\n", file) == EOF) {
        fclose(file);
        return -1;
    }

    if (fclose(file) != 0) {
        return -1;
    }

    return 0;
}

int kmemleak_read_text(const char *path, char *buffer, size_t buffer_size,
                       size_t *out_bytes) {
    FILE *file;
    size_t read_bytes;

    if (path == NULL || buffer == NULL || buffer_size < 2U || out_bytes == NULL) {
        return -1;
    }

    file = fopen(path, "r");
    if (file == NULL) {
        return -1;
    }

    read_bytes = fread(buffer, 1U, buffer_size - 1U, file);
    if (ferror(file) != 0) {
        fclose(file);
        return -1;
    }
    buffer[read_bytes] = '\0';
    *out_bytes = read_bytes;

    if (fclose(file) != 0) {
        return -1;
    }
    return 0;
}

int kmemleak_analyze_text(const char *text, kmemleak_report_t *out_report) {
    kmemleak_report_t report;
    const char *cursor;

    if (text == NULL || out_report == NULL) {
        return -1;
    }

    memset(&report, 0, sizeof(report));
    cursor = text;

    while (*cursor != '\0') {
        const char *line_end = strchr(cursor, '\n');
        size_t line_len = 0U;
        char line[256];

        if (line_end != NULL) {
            line_len = (size_t)(line_end - cursor);
        } else {
            line_len = strlen(cursor);
        }

        if (line_len >= sizeof(line)) {
            line_len = sizeof(line) - 1U;
        }
        memcpy(line, cursor, line_len);
        line[line_len] = '\0';

        if (strstr(line, "unreferenced object") != NULL) {
            size_t parsed_size = 0U;
            report.suspected_objects++;
            if (parse_size_from_header(line, &parsed_size)) {
                report.suspected_bytes += parsed_size;
                report.parsed_size_entries++;
            }
        }

        if (line_end == NULL) {
            break;
        }
        cursor = line_end + 1;
    }

    *out_report = report;
    return 0;
}

int kmemleak_analyze_file(const char *path, kmemleak_report_t *out_report) {
    char *buffer;
    size_t bytes_read = 0U;
    int rc;

    if (path == NULL || out_report == NULL) {
        return -1;
    }

    buffer = (char *)malloc(KMEMLEAK_BUFFER_SIZE);
    if (buffer == NULL) {
        return -1;
    }

    rc = kmemleak_read_text(path, buffer, KMEMLEAK_BUFFER_SIZE, &bytes_read);
    if (rc != 0) {
        free(buffer);
        return -1;
    }

    (void)bytes_read;
    rc = kmemleak_analyze_text(buffer, out_report);
    free(buffer);
    return rc;
}
