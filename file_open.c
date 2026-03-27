#include<stdio.h>
void main()
{
	FILE *fp=fopen("/dev/basic_char","w+" );
	perror("fopen");

	char s[]="85+100";

         fprintf(fp,"%s",s);

	 char s1[10];

	fscanf(fp,"%s",s1);

	printf("%s\n",s1);
	
	fclose(fp);
}
