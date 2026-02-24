#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <dirent.h>
#include <stdlib.h>

int main(int argc,char **argv) {
	DIR * dp= opendir(argv[1]);
	struct dirent *dir;
	char *files[100];
	int i = 0;
	while((dir = readdir(dp)) != NULL) {
		files[i] = dir->d_name;
		printf("%s\n",files[i]);
		i++;
	}
}

