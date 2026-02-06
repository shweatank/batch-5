#include<stdio.h>
int main()
{
	int x,y;
	scanf("%d%d",&x,&y);
	if((x^y)<0)
	{
		printf("one +ve,one -ve\n");

	}
	else
	{
		printf("both same sign\n");
	}
	return 0;
}
