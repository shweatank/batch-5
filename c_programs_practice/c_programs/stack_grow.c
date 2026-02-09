#include<stdio.h>

void fun(){
	int a;
	printf("%p\n",&a);
}

int main(){
	int n;
	printf("%p\n",&n);
	fun();
}


