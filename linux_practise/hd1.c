#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <string.h>

void traverse(char *path) {
	DIR *dp;
        dp = opendir(path);
	if(!dp) return;
        struct dirent *entry;
        while((entry = readdir(dp)) != NULL) {
		if(entry->d_type == DT_DIR) {
			if(strcmp(entry->d_name,".") == 0 || strcmp(entry->d_name,"..") ==0) continue;
			printf("\n%s is a directory file and its Inode is %ld and its files are:\n",entry->d_name,entry->d_ino);
			char path1[1024];
			snprintf(path1,sizeof(path1),"%s/%s",path,entry->d_name);
			traverse(path1);
			printf("\n");
		}
		else {
			printf("Filename: %s\tInode: %ld\n",entry->d_name,entry->d_ino);
		}
        }
        closedir(dp);
}

int main(int argc,char **argv) {
	if(argc != 2) {
		printf("Invalid Arguments..\n");
		return 0;
	}
	traverse(argv[1]);
}

