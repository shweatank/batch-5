#include<stdio.h>
void main()
{
int num;
printf("enter the num\n");
scanf("%d",&num);
int pos,val,c=0,i,t;

for(pos=31;pos>=0;pos--)
printf("%d ",num>>pos&1);

printf("enter the val\n");
scanf("%d",&val);
printf("enter the pos\n");
scanf("%d",&pos);

for(int i=31;i>=0;i--)
{
if(val>>i&1)
{
t=i;
break;
}
}
printf("t=%d\n",t);

num=num|(val<<(pos-t));
//num=num&(val<<(pos-t));

for(pos=31;pos>=0;pos--)
printf("%d ",num>>pos&1);
printf("\n");
}
