#include"header.h"
void main()
{
int newfd;
newfd=dup(1);
printf("%d\n",newfd);
write(1,"hello\n",6);
write(newfd,"hai\n",5);
}
