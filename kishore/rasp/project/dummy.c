#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char **argv) {
    if(argc != 2) {
        return 1;
}

printf("Message from kernel is %s\n",argv[1]);
    return 0;
}

