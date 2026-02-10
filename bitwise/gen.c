#include<stdio.h>
int main(){
	int num,rst,rnd;
	scanf("%d%d%d",&num,&rst,&rnd);
	printf("%b\n",num);
	int x=(num)&((1<<(rnd-rst))-1);
	printf("extracted num is %b\n",x);
	x^=x;
	printf("xored num is %b\n",x);
	num&=~(x<<rst)|(x&num)&((1<<rnd-rst)-1);
	printf("%b",num);




}
