#include <stdio.h>
#include <stdlib.h>

int my_getline(char **line, size_t *len, FILE *fp) {
    int ch;
    size_t size = 0;

    *line = malloc(100);
    if (!*line) return -1;

    while ((ch = fgetc(fp)) != EOF && ch != '\n') {
        (*line)[size++] = ch;
        if (size >= 99) break;
    }

    (*line)[size] = '\0';
    *len = size;

    if (size == 0 && ch == EOF) return -1;
    return size;
}

int main() {
    char *line = NULL;
    size_t len;

    printf("Enter a line:\n");
    my_getline(&line, &len, stdin);

    printf("You entered: %s\n", line);
    free(line);
    return 0;
}

