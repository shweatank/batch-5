#include"header.h"
void main()
{
int ret;
ret=alarm(10);
printf("%d\n",ret);
sleep(10);
ret=alarm(2);
printf("%d\n",ret);
ret=alarm(5);
printf("%d\n",ret);


}
