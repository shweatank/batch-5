#include<stdio.h>
int main(){
	int a=11,b=30;
	printf("%b\n%b\n",a,b);
	a=a^b;
	b=a^b;
	a=a^b;
	printf("%b\n%b\n",a,b);
}
