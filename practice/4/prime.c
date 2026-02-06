/*
Write a program to check data corresponding to first and fourth byte in a given integer is prime or not.
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
void data(char*p,int*num){
	int i=0;
	//	p=num;
	printf("Enter the num\n");
	for(i=0;i<4;i++){
		scanf("%x",p);
		p++;
	}
}
void prime(int*a,int*b){
	int num=0,i;
	char *p;
	p=&num;
/*	*p=*a;
printf("%d...\n",num);
	p++;
	*p=*b;
printf("%d...\n",num);
*/
num=*b;
num=num<<8;
printf("%d.......\n",num);
num=num|*a;
printf("num=%d\n",num);
	for(i=2;i<num;i++)
		if(num%i==0)
			break;
	if(num==i)
		printf("Prime\n");
	else
		printf("Not prime\n");
}
void main(){
	int num;
	char *p;
	p=&num;
	data(p,&num);
	printf("The number is %d\n",num);
	char *first,*last;
	first=&num;
	last=first+4;
	prime(first,last);
printf("1st=%d 4th=%d\n",*first,*last);
}

