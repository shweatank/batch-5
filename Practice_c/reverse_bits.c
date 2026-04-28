#include<stdio.h>
int main()
{
	int x=2147483648,temp1,temp2;
/*	for(int i=0,j=31;i<j;i++,j--)
	{
		temp1=(x>>i)&1;
		temp2=(x>>j)&1;
		if(temp1!=temp2)
		{
		   x^=(1<<i);
	   x^=1<<j;	   
		}
	}*/

	x=(((x&0xffff0000)>>16)|((x&0x0000ffff)<<16));
	x=(((x&0xff00ff00)>>8)|((x&0x00ff00ff)<<8));
	x=(((x&0xf0f0f0f0)>>4)|((x&0x0f0f0f0f)<<4));
	x=(((x&0xcccccccc)>>2)|((x&0x03030303)<<2));
	x=(((x&0xaaaaaaaa)>>1)|((x&0x55555555)<<1));
	printf("%d\n",x);
}
