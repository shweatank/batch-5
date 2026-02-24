#include <stdio.h>
#include <ctype.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <file>\n", argv[0]);
        return 1;
    }

    FILE *fp = fopen(argv[1], "r");
    if (!fp) {
        perror("File open failed");
        return 1;
    }

    int ch, lines = 0, words = 0, chars = 0;
    int in_word = 0;

    while ((ch = fgetc(fp)) != EOF) {
        chars++;
        if (ch == '\n') lines++;

        if (isspace(ch))
            in_word = 0;
        else if (!in_word) {
            in_word = 1;
            words++;
        }
    }

    fclose(fp);

    printf("Lines: %d\nWords: %d\nCharacters: %d\n", lines, words, chars);
    return 0;
}

