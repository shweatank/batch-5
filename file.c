#include<stdio.h>
#include<stdlib.h>
int main() {
    FILE *fp;
    char writeData[] = "Hello, this is file handling.";
    char readData[100];

	fp = fopen("example.txt", "w");
    if (fp == NULL) {
        printf("Error opening file for writing.\n");
        return 1;
    }

	fwrite(writeData, sizeof(char), sizeof(writeData), fp);

    fclose(fp);


    
    fp = fopen("example.txt", "r");
    if (fp == NULL) {
        printf("Error opening file for reading.\n");
        return 1;
    }

    fread(readData, sizeof(char), sizeof(writeData), fp);

    
    fclose(fp);

    
    printf("Data read from file:\n%s\n", readData);

    return 0;
}
