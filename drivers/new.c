#include<stdio.h>
int main(){
	FILE *fp=fopen("/dev/basic_char","r");
	char buff[50];
	while(1)
	{
	fread(buff,sizeof(buff),1,fp);
	fwrite(buff,sizeof(buff),1,fp);
	fclose(fp);
	}
}

