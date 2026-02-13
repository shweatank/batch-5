#include<stdio.h>
int main(){
	int n,k;
	scanf("%d%d",&n,&k);
	printf("%b\n",n);
	//n&=0;
	if((n>>k)&1)
		puts("set");
	else
		puts("not set");
	printf("%b",n);
}
