#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main() {
    int fd;
    char writeData[] = "Hello, file handling";
    char readData[100];

    
    fd = open("example.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        printf("Error opening file for writing.\n");
        return 1;
    }

  
    write(fd, writeData, strlen(writeData));

    
    close(fd);
	 fd = open("example.txt", O_RDONLY);
    if (fd < 0) {
        printf("Error opening file for reading.\n");
        return 1;
    }

    read(fd, readData, sizeof(readData));

    
    close(fd);

    
    printf("Data read from file:\n%s\n", readData);

    return 0;
}

