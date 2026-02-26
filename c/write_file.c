//write multiple lines into the file 

#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE *fp;
    char line[100];
    int i, n;

    fp = fopen("multiple_lines.txt", "w"); // Open in write mode ("w")
    if (fp == NULL) {
        printf("Error opening file for writing!\\n");
        exit(1);
    }

    printf("Enter the number of lines to write: ");
    scanf("%d", &n);
    
    while ((getchar()) != '\n'); // Consume the newline character left by scanf

    printf("Enter %d lines of text:\\n", n);
    for (i = 0; i < n; i++) {
        
	    fgets(line, sizeof(line), stdin); // Use fgets to read a full line from stdin, including spaces
        
	    fputs(line, fp); // Use fputs to write the line to the file
    }

    fclose(fp);
    printf("Successfully wrote to the file.\\n");

    return 0;
}

