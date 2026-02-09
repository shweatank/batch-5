#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define IOC_MAGIC 'M'

struct calc_req {
    int a;
    int b;
    long result;
    int err;
};

#define CALC_ADD _IOWR(IOC_MAGIC, 1, struct calc_req)
#define CALC_SUB _IOWR(IOC_MAGIC, 2, struct calc_req)
#define CALC_MUL _IOWR(IOC_MAGIC, 3, struct calc_req)
#define CALC_DIV _IOWR(IOC_MAGIC, 4, struct calc_req)

int main() {
    int fd = open("/dev/mycalc", O_RDWR);
    if (fd < 0) { perror("open"); return 1; }

    struct calc_req req;
    char op;

    printf("Enter operation (a op b), e.g., 5 + 3: ");
    if (scanf("%d %c %d", &req.a, &op, &req.b) != 3) return 1;

    unsigned int cmd;
    switch (op) {
        case '+': cmd = CALC_ADD; break;
        case '-': cmd = CALC_SUB; break;
        case '*': cmd = CALC_MUL; break;
        case '/': cmd = CALC_DIV; break;
        default: printf("Unknown operator\n"); return 1;
    }

    if (ioctl(fd, cmd, &req) < 0) { perror("ioctl"); return 1; }

    if (req.err)
        printf("Error: divide by zero\n");
    else
        printf("Result: %ld\n", req.result);

    close(fd);
    return 0;
}

