#include<stdio.h>
#include<stdlib.h>
void main(){

	int *p;
	long int a;
	while(1){
		p=malloc(101);
		a=(long int)p;
		printf(".....%p\n",p);
		if(a%16==0)
			break;
		else{
			free(p);
		}
	}

	printf("%p\n",p);
}
