#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <word> <file>\n", argv[0]);
        return 1;
    }

    FILE *fp = fopen(argv[2], "r");
    if (!fp) {
        perror("File open failed");
        return 1;
    }

    char line[512];
    int line_no = 1;

    while (fgets(line, sizeof(line), fp)) {
        if (strstr(line, argv[1])) {
            printf("%d: %s", line_no, line);
        }
        line_no++;
    }

    fclose(fp);
    return 0;
}

