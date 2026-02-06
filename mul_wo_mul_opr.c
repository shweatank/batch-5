#include<stdio.h>
int main()
{
	int x,y,neg=0;
	long int res=0;
	scanf("%d%d",&x,&y);
	for(int i=31;i>=0;i--)printf("%d ",(x>>i)&1);
	printf("\n");
	for(int i=31;i>=0;i--)printf("%d ",(y>>i)&1);
	printf("\n");
	if(x<0){neg^=1;x=-x;}
	if(y<0){neg^=1;y=-y;}
	while(x)
	{
		if(x&1)
		{
			res+=y;
		}
		y<<=1;
		x>>=1;
		
	}
	if(neg)
	{res=-res;
	}

	printf("%ld is multiplied result\n",res);

}
