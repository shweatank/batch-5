#include <stdio.h>
#include <string.h>

char* my_strtok(char* str, const char* delim) {
    static char* next_token = NULL;
    if (str != NULL) {
        next_token = str;
    }
    
    // If no more tokens remain
    if (next_token == NULL || *next_token == '\0') {
        return NULL;
    }

    // Skip leading delimiters
    while (*next_token && strchr(delim, *next_token)) {
        next_token++;
    }

    // Check if skipping delimiters reached the end of the string
    if (*next_token == '\0') {
        return NULL;
    }

    char* token_start = next_token;

    // Find the end of the current token
    while (*next_token && !strchr(delim, *next_token)) {
        next_token++;
    }

    // If a delimiter was found, terminate the token and advance next_token
    if (*next_token != '\0') {
        *next_token = '\0';
        next_token++;
    }

    return token_start;
}

int main() {
    char input[] = "Hello, world; this is C.";
    char* token = my_strtok(input, " ,;.");
    
    while (token != NULL) {
        printf("Token: %s\n", token);
        token = my_strtok(NULL, " ,;.");
    }
    
    return 0;
}

