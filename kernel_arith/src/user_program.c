#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>

int main() {
    int fd;
    int nums[2];
    char buf[128];

    printf("Enter first number: ");
    scanf("%d", &nums[0]);
    printf("Enter second number: ");
    scanf("%d", &nums[1]);

	fd = open("/dev/arith", O_RDWR);
    if (fd < 0) {
        perror("Failed to open device");
        return -1;
    }

    
    if (write(fd, nums, sizeof(nums)) < 0) {
        perror("Write failed");
        close(fd);
        return -1;
    }

  
    if (read(fd, buf, sizeof(buf)) < 0) {
        perror("Read failed");
        close(fd);
        return -1;
    }

    printf("Results from kernel:\n%s", buf);

    close(fd);
    return 0;
	
	}
