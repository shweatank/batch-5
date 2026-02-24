#include <stdio.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <source_file> <destination_file>\n", argv[0]);
        return 1;
    }

    FILE *src = fopen(argv[1], "rb");
    if (src == NULL) {
        printf("Error: Cannot open source file\n");
        return 1;
    }

    FILE *dest = fopen(argv[2], "wb");
    if (dest == NULL) {
        printf("Error: Cannot open destination file\n");
        fclose(src);
        return 1;
    }

    char buffer[1024];
    size_t bytes;

    while ((bytes = fread(buffer, 1, sizeof(buffer), src)) > 0) {
        fwrite(buffer, 1, bytes, dest);
    }

    fclose(src);
    fclose(dest);

    printf("File copied successfully using C file concepts\n");
    return 0;
}
