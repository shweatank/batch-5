#include"header.h"
void main()
{
FILE*fp=fopen("/dev/basic_char","w");
char s[10]="saikumar";
int a=10000;
//fwrite(s,sizeof(char)*10,1,fp);
fwrite(&a,4,1,fp);
fclose(fp);
}



