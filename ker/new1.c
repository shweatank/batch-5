#include<stdio.h>
#include<string.h>
int main(){
	FILE *fp;
	fp=fopen("/dev/read_write","w+");
	char buff[90],buff1[90];
	strcpy(buff,"Hey there in the device file");
	int n=fwrite(buff,sizeof(buff),1,fp);
	if(n!=1)
		perror("fwrite");
	printf("num of bytes written %d",n);
	fread(buff1,sizeof(buff1),1,fp);
	fclose(fp);
}
