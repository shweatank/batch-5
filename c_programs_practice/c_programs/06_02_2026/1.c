/*Write a program to check data corresponding to first and fourth byte in a given integer is prime or not.
Use Following function prototype.
Void data(char *,int *);
To make the data you have to use char * only.
Void primecheck(int*,int *);
 Take number from user.
Write a generic code.
Example:
int num=0x01000001;
Now data corresponding to first and fourth byte is 257 which is prime.
Output = Prime.*/

#include<stdio.h>
#include<stdlib.h>

void data(char *n,int *dat){
	*dat=0;
	*dat+=*n;
	n=n+3;
	*dat+=*n;
}

void prime(int *n){
	int i;
	for(i=2;i<*n;i++){
		if(*n%i==0)
			break;
	}
	if(i==*n)
		printf("%d is PRIME\n",*n);
	else
		printf("%d is not PRIME\n",*n);

}

int main(){
	unsigned int n,dat;
	scanf("%x",&n);
	printf("%x %d\n",n,n);
	data((char*)&n,&dat);
	printf("%d\n",dat);
	prime(&dat);
}


