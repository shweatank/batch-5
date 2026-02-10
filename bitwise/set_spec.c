#include<stdio.h>
int main(){
	int n=30,i;
	scanf("%d",&i);
	printf("%d\n",n);
	n=n|(1<<i);
	printf("%d\n",n);
}

