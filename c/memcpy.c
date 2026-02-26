#include <stdio.h>
#include <string.h>

int main() {
    char src[] = "Hello, World!";
    char dest[20];

    memcpy(dest, src, strlen(src) + 1);
    printf("Copied string: %s\n", dest);

    return 0;
}

