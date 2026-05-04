#ifndef INPUT_H
#define INPUT_H

#include <stddef.h>

/* Reads a line (without trailing newline). Returns 0 on success. */
int input_read_line(const char *prompt, char *buf, size_t buf_len);

/* Strict integer parse from a line. Returns 0 on success. */
int input_read_int(const char *prompt, int *out_value, int min_value, int max_value);

#endif
