#include"header.h"
void main()
{
int n1,n2;
mkfifo("f1",0664);
perror("mkfifo");
mkfifo("f2",0664);
perror("mkfifo");
int fdw=open("f1",O_RDWR);
perror("open");
int fdr=open("f2",O_RDWR);
perror("open");
while(1)
{
printf("enter number.. n1\n");
scanf("%d",&n1);
write(fdw,&n1,4);
read(fdr,&n2,4);
printf("read from process anotheri n2= %d\n",n2);
}
}
