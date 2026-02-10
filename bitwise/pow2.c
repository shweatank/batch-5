#include<stdio.h>
int main(){
	int n,c=0;
	scanf("%d",&n);
	/*while(n>0){
	n>>=1;
	if(n&1)
		c++;
	}
	if(c==1)
		 printf("is a power of 2");*/
	//if((n&(n-1))==0)
	//while(n>0 && (n&~(n-1))==n)
	while(n>1 && n%2==0){
		n/=2;
	}if(n==1)	
		printf("is a power of 2");
}

