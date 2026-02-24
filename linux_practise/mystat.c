#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>

int mystat(const char *pathname,struct stat *statbuf) {
	if(stat(pathname,statbuf) == -1) {
		printf("Unable to open the file..!");
		return 0;
	}
	printf("File: %s\n",pathname);
	printf("Size: %ld\tBlocks: %ld\tIO Block: %ld\t",statbuf->st_size,statbuf->st_blocks,statbuf->st_blksize);
	int filetype = statbuf->st_mode & S_IFMT;
 	switch(filetype) {
		case S_IFSOCK:
                        printf("socket\n");
                        break;
		case S_IFLNK:
                        printf("symbolic link\n");
                        break;
		case S_IFREG:
                        printf("regular file\n");
                        break;
		case S_IFBLK:
                        printf("block device\n");
                        break;
		case S_IFDIR:
                        printf("directory\n");
                        break;
		case S_IFCHR:
                        printf("character device\n");
                        break;
		case S_IFIFO:
                        printf("FIFO\n");
                        break;
	}
	printf("Device: %ld\tInode: %ld\tLinks: %ld\n",statbuf->st_dev,statbuf->st_ino,statbuf->st_nlink);
	printf("Access: (%04o)\tUid: (%d)\tGid: (%d)\n",statbuf->st_mode & 07777,statbuf->st_uid,statbuf->st_gid);
	 printf("Access: %s", ctime(&statbuf->st_atime));
   	 printf("Modify: %s", ctime(&statbuf->st_mtime));
   	 printf("Change: %s", ctime(&statbuf->st_ctime));
//	 printf("Birth: %s", ctime(statbuf->st_birthtime));

}

int main(int argc,char **argv) {
	if(argc != 2) {
		printf("Invalid Arguments\n");
		return 0;
	}
	struct stat statbuf;
	mystat(argv[1],&statbuf);
}
