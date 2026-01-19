#include"header.h"
struct student
{
int rno;
char name[20];
}v;
int p[2];
void main()
{
pipe(p);
if(fork()==0)
{
int ret;
struct student v1;
ret=read(p[0],&v1,sizeof(v1));
printf("%d\n",ret);
printf("%d %s\n",v1.rno,v1.name);
}
else
{
int ret;
v.rno=10;
strcpy(v.name,"saikumar");
ret=write(p[1],&v,sizeof(v));
printf("%d\n",ret);
}
}
