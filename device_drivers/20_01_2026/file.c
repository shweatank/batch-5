#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(){
	char s[100];
	char buff[100];
//	scanf("%[^\n]",s);
	FILE *fp=fopen("/dev/basic_char","w");
	fputs("file",fp);
	//fwrite(s,strlen(s)+1,1,fp);
	//fseek(fp,-strlen(s),1);
	fclose(fp);
	
	//fp=fopen("/dev/basic_char","r");
	//fgets(buff,sizeof(buff),fp);
	//fread(buff,sizeof(buff),1,fp);
	//printf("%s\n",buff);
//	fclose(fp);
}

