#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// Convert digit character to corresponding word
const char* digit_to_word(char c) {
    switch(c) {
        case '0': return "zero";
        case '1': return "one";
        case '2': return "two";
        case '3': return "three";
        case '4': return "four";
        case '5': return "five";
        case '6': return "six";
        case '7': return "seven";
        case '8': return "eight";
        case '9': return "nine";
        default: return NULL;
    }
}

// Read a string of arbitrary length from user
char* getstring() {
    int i = 0;
    char *p = NULL;
    char ch;

    while ((ch = getchar()) != '\n' && ch != EOF) {
        p = realloc(p, i + 2); // +1 for new char, +1 for '\0'
        if (!p) {
            printf("Memory allocation failed\n");
            exit(1);
        }
        p[i++] = ch;
    }
    if (p)
        p[i] = '\0';
    return p;
}

int main() {
    printf("Enter a string: ");
    char *str = getstring();
    if (!str) {
        printf("No input received.\n");
        return 1;
    }

    // Allocate an array of char* pointers to hold converted words
    size_t len = strlen(str);
    char **op = malloc(len * sizeof(char*));
    if (!op) {
        printf("Memory allocation failed\n");
        free(str);
        return 1;
    }

    for (size_t i = 0; i < len; i++) {
        if (isdigit(str[i])) {
            op[i] = strdup(digit_to_word(str[i]));  // copy word to new memory
        } else {
            op[i] = malloc(2);  // one char + '\0'
            if (op[i]) {
                op[i][0] = str[i];
                op[i][1] = '\0';
            }
        }
    }

    printf("Converted output: ");
    for (size_t i = 0; i < len; i++) {
        printf("%s", op[i]);
        free(op[i]);  // Free each string after use
    }
    printf("\n");

    free(op);
    free(str);
    return 0;
}

