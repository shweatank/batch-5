#include<stdio.h>

int sum(int a,int b){
	printf("%p %p\n",&a,&b);
	return a+b;
}

int main(){
	int a,b;
	printf("%d",sum(4,3));
}

