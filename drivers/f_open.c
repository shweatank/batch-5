
#include <stdio.h>
#include <string.h>
/*
char *itoa(int num) {
        char *res = NULL;
        int count = 0;
        while(num) {
                res = realloc(res,count+1);
                res[count] = num%10;
                count++;
                num /= 10;
        }
        res[count] = '\0';
        reverse(res);
        return res;
}
*/


struct kbuf {
        int res;
        char op;
};

static char *data;

int main() {
	FILE *fp = fopen("/proc/proc_basic","r+");
	perror("fopen: ");
	char str[100];
	printf("Enter the numbers and operation in string: ");
	scanf("%99s",str);
	fwrite(str,strlen(str)+1,1,fp);
	perror("fwrite: ");
	
	fread(&data,3,1,fp);
	perror("fread: ");
	printf("result: %s\n",data);
//	printf("Operation done: %c\n",data.op);
	fclose(fp);
	return 0;
}

/*

#include <stdio.h>
#include <string.h>

int main() {
    FILE *fp = fopen("/dev/basic_char", "r+");
    if (fp == NULL) {
        perror("fopen");
        return 1;
    }

    char *str = "This is test text to be written in char dev file";

    if (fwrite(str, 1, strlen(str) + 1, fp) != strlen(str) + 1) {
        perror("fwrite");
        fclose(fp);
        return 1;
    }

    char res[strlen(str) + 1];
    fseek(fp, 0, SEEK_SET);

    if (fread(res, 1, strlen(str) + 1, fp) <= 0) {
        perror("fread");
        fclose(fp);
        return 1;
    }

    printf("Read: %s\n", res);

    fclose(fp);
    return 0;
}
*/
