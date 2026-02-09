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
#include<stdlib.h>
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
	int i;
	printf("*a=%d *b=%d\n",*a,*b);
*b=(*b<<8);
	for(i=2;i<((*a)|(*b));i++)
		if(((*a)|(*b))%i==0)
			break;
	if(((*a)|(*b))==i)
		printf("Prime\n");
	else
		printf("Not prime\n");
}
void main(){
	int num;
	char *p;
	p=&num;
	data(p,&num);
p=&num;
	printf("The number is %x\n",num);
	int *first=malloc(sizeof(int));
	int *last=malloc(sizeof(int));
	*first=*p;p=p+3;
	*last=*p;
	prime(first,last);
}

