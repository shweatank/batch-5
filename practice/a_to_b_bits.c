#include<stdio.h>
int main()
{
	int x,y,res=0,cnt=0;
	scanf("%d%d",&x,&y);
	for(int i=31;i>=0;i--)printf("%d ",(x>>i)&1);
	printf("\n");
	for(int i=31;i>=0;i--)printf("%d ",(y>>i)&1);
	printf("\n");
	res=x^y;
	while(res)
	{
		res=res&(res-1);
		cnt++;
	}
	printf("%d needs to be modified to change x to y\n",cnt);

}
