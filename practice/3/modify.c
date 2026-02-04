#include<stdio.h>
void main(){

	int a,b;
	printf("Enter the numbers a and b\n");
	scanf("%d%d",&a,&b);
	printf("Before : a=%d,b=%d\n",a,b);
	int n1=a,n2=b;

	for(int pos=31;pos>=0;pos--){
		if( (a&(1<<pos))!=(b&(1<<pos)))
			a=a^(1<<pos);
	}
	printf("After: a=%d,b=%d\n",a,b);
}
