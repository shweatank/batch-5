#include"header.h"
void ISR(int n)
{
FILE*fp=fopen("data","w");
fprintf(fp,"hello\n");
fclose(fp);
}
void main()
{
printf("hello pid=%d\n",getpid());
signal(SIGHUP,ISR);
while(1);
}
