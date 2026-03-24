#include"header.h"
int setbits(int x,int p,int n ,int y)
{
int i,dy=0;
for(i=0;i<n;i++,p++)
{
if((x>>p&1)!=(y>>i&1))
x^=(1<<p);
}
return x;
}
void main()
{
int x,p,y,n;
printf("enter number:\n");
scanf("%d",&x);
printf("enter position:\n");
scanf("%d",&p);
printf("enter how many bits you want to modify:\n");
scanf("%d",&n);
printf("enter new number:\n");
scanf("%d",&y);
int ret=setbits(x,p,n,y);
for(int pos=31;pos>=0;pos--)
{
printf("%d",x>>pos&1);
if(pos%4==0)
printf(" ");
}
printf("\n");
for(int pos=31;pos>=0;pos--)
{
printf("%d",y>>pos&1);
if(pos%4==0)
printf(" ");
}
printf("\n");
for(int pos=31;pos>=0;pos--)
{
printf("%d",ret>>pos&1);
if(pos%4==0)
printf(" ");
}
printf("\n");
}
