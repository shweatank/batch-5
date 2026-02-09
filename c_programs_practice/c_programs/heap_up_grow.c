#include<stdio.h>
#include<stdlib.h>

int main(){
	int *p=malloc(2);
	int *m=malloc(2);
	printf("%p %p\n",p,m);
}

