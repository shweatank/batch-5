#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
void main()
{
	char s[]="welcome";
	FILE *fd=fopen("/dev/reverse_dev","w+");
	fprintf(fd,"%s",s);
	printf("before:%s\n",s);
	fscanf(fd,"%s",s);
        
	printf("After:%s\n",s);
}












































/*#define DEVICE "/dev/basic"
#define BUF_SIZE 256

int main() {
    int fd;
    char input[BUF_SIZE];
    char reversed[BUF_SIZE];
    ssize_t n;

    // Open the character device
    fd = open(DEVICE, O_RDWR);
    if (fd < 0) {
        perror("Failed to open device");
        return 1;
    }

    // Get input from user
    printf("Enter a string to reverse: ");
    if (!fgets(input, sizeof(input), stdin)) {
        printf("Error reading input\n");
        close(fd);
        return 1;
    }

    // Remove newline if present
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n')
        input[len - 1] = '\0';

    // Write string to kernel device
    n = write(fd, input, strlen(input));
    if (n < 0) {
        perror("Failed to write to device");
        close(fd);
        return 1;
    }

    // Read reversed string from kernel device
    n = read(fd, reversed, sizeof(reversed) - 1);
    if (n < 0) {
        perror("Failed to read from device");
        close(fd);
        return 1;
    }

    reversed[n] = '\0';  // null-terminate

    printf("Reversed string from kernel: %s\n", reversed);

    close(fd);
    return 0;
}
*/
