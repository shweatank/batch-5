#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

int main(int argc,char **argv) {
	if(argc != 4) {
		puts("Invalid Arguments..!");
		return 0;
	}
	int writeBytes = atoi(argv[2]);
	int readBytes = atoi(argv[3]);
	int fifod;
	char str[writeBytes];
	memset(str,'*',writeBytes-1);
	str[writeBytes] = '\0';
	int ret = mkfifo(argv[1],0644);
	if(ret == -1) {
		puts("FIFO error..!");
		return 0;
	}
	fifod = open(argv[1],O_RDWR);
	if(fork() == 0) {
		lseek(fifod,0,SEEK_CUR);
		char s[readBytes+1];
		read(fifod,s,readBytes);
		s[readBytes] = '\0';
		printf("%s\n",s);
	}
	else {
		write(fifod,str,writeBytes);
	}
}


/*
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(int argc, char **argv) {
    if (argc != 3) {
        puts("Invalid Arguments..!");
        return 0;
    }

    int n = atoi(argv[2]);
    if (n <= 0) return 0;

    if (mkfifo(argv[1],O_RDWR) == -1) {
        perror("mkfifo");
    }

    int fifod = open(argv[1], O_RDWR);
    if (fifod == -1) {
        perror("open");
        return 0;
    }

    if (fork() == 0) {
        char s[n + 1];
        read(fifod, s, n);
        s[n] = '\0';
        printf("%s\n", s);
    } else {
        char str[n + 1];
        memset(str, '*', n);
        str[n] = '\0';
        write(fifod, str, n);
    }

    close(fifod);
    unlink(argv[1]);
    return 0;
}
*/
