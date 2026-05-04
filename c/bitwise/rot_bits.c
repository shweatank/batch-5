#include<stdio.h>
void main()
{
unsigned int num;
int pos,r,n;
printf("enter the num\n");
scanf("%d",&num);
printf("enter the no of bits uh want to rotate\n");
scanf("%d",&n);

for(pos=31;pos>=0;pos--)
printf("%d ",num>>pos&1);
printf("\n");
for(int i=0;i<n;i++)
{
r=num&1;
num=num>>1;
if(r)
num=num|1<<31;
}

for(pos=31;pos>=0;pos--)
printf("%d ",num>>pos&1);
printf("\n");
}
