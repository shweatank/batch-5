#include"header.h"
void main()
{
int n1,n2;
int fdr=open("f1",O_RDWR);
perror("open");
int fdw=open("f2",O_RDWR);
perror("open");
while(1)
{
read(fdr,&n1,4);
printf("read from process another n1= %d\n",n1);
printf("enter number.. n2\n");
scanf("%d",&n2);
write(fdw,&n2,4);
}
}
