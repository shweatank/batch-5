#include<stdio.h>
int main(){
	int i=1,x=5,y=3;
	printf("%d\n",i<<31);
	printf("%d\n",x&y);
	x=7;
	printf("%d\n",x&(x-1));
	x=8;
	printf("%d\n",x>>1);
	unsigned char c=255;
	printf("%d\n",c<<1);
	x=-1;
	printf("%d\n",~x);

}
