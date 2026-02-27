#include<stdio.h>
int main()
{
	int n,ch;
	printf("enter num:");scanf("%d",&n);
	printf("choice:0-mul 1-div");scanf("%d",&ch);
	if(ch==0)
	{
		n=n<<1;
	}
	else
	{
		n=n>>1;
	}
	printf("updated num:%d\n",n);
	return 0;

}
