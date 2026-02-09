#include<stdio.h>

int main(){
	int n=9;
	void *p=&n;
	printf("%p\n",p);
	p++;//p=p+1*sizeof(void)
	printf("%p\n",p);
}


