#include<stdio.h>
int main(){
	int n,i,k,c=0;
	scanf("%d",&n);
	printf("%b\n",n);
	/*for(i=31;i>=0;i--){
		if((n>>i)&1)
			c++;
	}*/
	//i=31;
	while(n>0){
		k=n&1;
		if(k)
			c++;
		n=(n>>1);
	}
	/*while(n>0){
		n&=(n-1);
		c++;
	}*/
	
	printf("cnt is %d\n",c);
	//printf("%d\n",((n>>i)&1));
}

