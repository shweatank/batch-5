#include<stdio.h>
int main()
{
	int n,start,end,mask;
	scanf("%d",&n);printf("range: low,high:");
	scanf("%d%d",&start,&end);
	mask=(((1<<(end-start+1))-1)<<start);
	n|=mask;
	printf("num:%d\n",n);  
}
