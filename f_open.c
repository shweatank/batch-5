#include<stdio.h>
int main()
{
	FILE *fd=fopen("/dev/basic_char","w");
	perror("fopen");
	char s[20]="hello";
	fputs(s,fd);
	
	fclose(fd);
	fd=fopen("/dev/basic_char","r");
	char c[20];
	if(fgets(c,20,fd)!=0)
		printf("%s",c);
	fclose(fd);
	return 0;
}
