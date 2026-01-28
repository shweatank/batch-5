#include<stdio.h>
#include<stdlib.h>

int main(){
	char s[100];
	char buff[100];
	FILE *fp=fopen("/dev/reverse_dev","w");
	scanf("%[^\n]",s);
	fputs(s,fp);
	fclose(fp);

	fp=fopen("/dev/reverse_dev","r");
	fgets(buff,sizeof(buff),fp);
	printf("%s\n",buff);

}

