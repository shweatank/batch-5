#include"header.h"
void main()
{
int p[2];
//pipe2(p,O_NONBLOCK);
pipe(p);
if(fork()==0)
{
	int i=10;
	write(p[1],&i,4);
}
else
{
	if(fork()==0)
	{
		int i;
		sleep(1);
		read(p[0],&i,4);

	printf("i=%d\n",i);
	}
	else
	{
	sleep(2);
  	int a[2],ret;
ret=	read(p[0],a,8);
printf("%d %d  ret=%d\n",a[0],a[1],ret);
	}


}




}
