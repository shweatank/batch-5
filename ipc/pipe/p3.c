#include"header.h"
int p[2];
void main()
{
pipe(p);
if(fork()==0)
{
int n,i;
close(p[1]);
n=read(p[0],&i,4);
printf("%d\n",n);
}
else
{
int i;
read(p[0],&i,4);
}
}
