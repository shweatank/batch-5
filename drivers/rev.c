#include<stdio.h>
#include<string.h>
#include<stdlib.h>
void main()
{
   FILE *fp;
   fp=fopen("/dev/basic","w+");
   char s[10]="hello",*p=calloc(strlen(s),1);
   fwrite(s,sizeof(s),1,fp);
   fread(p,sizeof(p),1,fp);
printf("%s",p);
   fclose(fp);
}

