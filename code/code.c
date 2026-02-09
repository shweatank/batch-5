#include <stdio.h>

int main() {
    FILE *f1, *f2, *f3;
    char text[100];

  
    printf("Enter text for file1: ");
    fgets(text, sizeof(text), stdin);

    f1 = fopen("file1.txt", "w");
    fputs(text, f1);
    fclose(f1);

    
    printf("Enter text for file2: ");
    fgets(text, sizeof(text), stdin);

    f2 = fopen("file2.txt", "w");
    fputs(text, f2);
    fclose(f2);

   
    printf("Enter text for file3: ");
    fgets(text, sizeof(text), stdin);

    f3 = fopen("file3.txt", "w");
    fputs(text, f3);
    fclose(f3);

    
    char ch;

    printf("\n---- file1.txt ----\n");
    f1 = fopen("file1.txt", "r");
    while ((ch = fgetc(f1)) != EOF)
        putchar(ch);
    fclose(f1);

    printf("\n---- file2.txt ----\n");
    f2 = fopen("file2.txt", "r");
    while ((ch = fgetc(f2)) != EOF)
        putchar(ch);
    fclose(f2);

    printf("\n---- file3.txt ----\n");
    f3 = fopen("file3.txt", "r");
    while ((ch = fgetc(f3)) != EOF)
        putchar(ch);
    fclose(f3);

    return 0;
}

