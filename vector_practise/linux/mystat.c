#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

int main(int argc, const char *argv[])
{
    if (argc != 2)
    {
        printf("Invalid Arguments..!\n");
        return 0;
    }

    struct stat filedata;

    if (stat(argv[1], &filedata) == -1)
    {
        perror("fetching file data failed");
        return 0;
    }

    printf("File: %s\n", argv[1]);

    printf("Size: %ld\tBlocks: %ld\tIO Block: %ld\t",
           (long)filedata.st_size,
           (long)filedata.st_blocks,
           (long)filedata.st_blksize);

    // File type
    if (S_ISREG(filedata.st_mode)) printf("regular file\n");
    else if (S_ISDIR(filedata.st_mode)) printf("directory\n");
    else if (S_ISCHR(filedata.st_mode)) printf("character device\n");
    else if (S_ISBLK(filedata.st_mode)) printf("block device\n");
    else if (S_ISFIFO(filedata.st_mode)) printf("FIFO\n");
    else if (S_ISLNK(filedata.st_mode)) printf("symbolic link\n");
    else if (S_ISSOCK(filedata.st_mode)) printf("socket\n");

    printf("Device: %ld\tInode: %ld\tLinks: %ld\n",
           (long)filedata.st_dev,
           (long)filedata.st_ino,
           (long)filedata.st_nlink);

    printf("Access: (%04o)\tUid: (%d)\tGid: (%d)\n",
           filedata.st_mode & 07777,
           filedata.st_uid,
           filedata.st_gid);

    // Time printing
    printf("Access: %s", ctime(&filedata.st_atime));
    printf("Modify: %s", ctime(&filedata.st_mtime));
    printf("Change: %s", ctime(&filedata.st_ctime));

#ifdef st_birthtime   // Some Linux systems do not support birthtime
    printf("Birth: %s", ctime(&filedata.st_birthtime));
#endif

    return 0;
}

