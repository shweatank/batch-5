#include<stdio.h>
void main()
{
	unsigned int num;
	int pos,s,e;
	printf("enter the num\n");
	scanf("%d",&num);
	printf("enter start pos\n");
	scanf("%d",&s);
	printf("enter the end pos\n");
	scanf("%d",&e);
	for(pos=31;pos>=0;pos--)
		printf("%d ",num>>pos&1);
	printf("\n");
	{
		num=num<<(31-e);
		num=num>>(31-e);
		num=num>>s;
	}

	/*int temp=(1<<(e+1))-1 & num;
	  temp=temp>>s;*/
	printf("num=%d\n",num);
}
