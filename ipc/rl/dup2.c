#include"header.h"
void main()
{
int newfd;
newfd=dup2(1,9);
printf("%d\n",newfd);
write(1,"hello\n",10);
write(9,"hai\n",5);
}
