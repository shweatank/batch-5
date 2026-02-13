#include<stdio.h>
int main(){
	int n,pos=0,mask,k;
	scanf("%d",&n);
	printf("%b\n",n);
//	n=n|(0xAAAAAAAA);
	k=n;
	while(k){
		if(k>>=1)
			pos++;
	}
	printf("%d\n",pos);
	mask=((1<<(pos+1))-1)&(0xAAAAAAAA);
	printf("%b\n",mask);
	n|=mask;
	printf("%b",n);
}
