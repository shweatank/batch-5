#include<stdio.h>
void main()
{
unsigned int num;
int s,e,pos;
printf("enter the num\n");
scanf("%d",&num);
printf("enter the start pos\n");
scanf("%d",&s);
printf("enter the end pos\n");
scanf("%d",&e);
for(pos=31;pos>=0;pos--)
printf("%d ",num>>pos&1);
printf("\n");

unsigned int r,r1;
r=num<<(32-s);
r=r>>(32-s);
r1=num>>(e+1);
r1=r1<<(e+1);

num=r1|r;

for(pos=31;pos>=0;pos--)
printf("%d ",num>>pos&1);
}


