#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define CALC_IOC_MAGIC 'C'

#define CALC_IOC_ADD  _IOWR(CALC_IOC_MAGIC, 1, struct calc_req)
#define CALC_IOC_SUB  _IOWR(CALC_IOC_MAGIC, 2, struct calc_req)
#define CALC_IOC_MUL  _IOWR(CALC_IOC_MAGIC, 3, struct calc_req)
#define CALC_IOC_DIV  _IOWR(CALC_IOC_MAGIC, 4, struct calc_req)
#define CALC_IOC_MOD  _IOWR(CALC_IOC_MAGIC, 5, struct calc_req)

struct calc_req {
    int a;
    int b;
    long result;  
    int err;       
};

int main(void)
{
    

    struct calc_req v;
   v.a=10;
   v.b=20;

    int fd = open("/dev/ioct_calc", O_RDWR);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    printf("User: sending %d  %d to kernel\n", v.a,v.b);

    ioctl(fd, CALC_IOC_SUB, &v);

    printf("User: got back %ld from kernel\n", v.result);

    
    printf("User: sending %d  %d to kernel\n", v.a,v.b);

    ioctl(fd, CALC_IOC_MUL, &v);

    printf("User: got back %ld from kernel\n", v.result);


    printf("User: sending %d  %d to kernel\n", v.a,v.b);

    ioctl(fd, CALC_IOC_ADD, &v);

    printf("User: got back %ld from kernel\n", v.result);

    close(fd);
    return 0;
}

