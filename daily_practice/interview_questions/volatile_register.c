#include<stdio.h>
int main()
{
	 register int x,y=20;
	x=10;
	printf("%d\n",++x+x++);
	printf("%d\n",++x+y++);

}
