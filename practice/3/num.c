/*

input:12345
output:24135
*/

#include<stdio.h>
void main(){

	int num;
	printf("Enter the number\n");
	scanf("%d",&num);
	printf("Before=%d\n",num);
	int temp,res=0,r,rev=num;
	for(temp=rev,num=0;temp;temp=temp/10){
		r=temp%10;
		num=num*10+r;
	}
	for(temp=num;temp;temp=temp/10){
		r=temp%10;
		if(r%2==0)
			res=res*10+r;
	}
	for(temp=num;temp;temp=temp/10){
		r=temp%10;
		if(r%2!=0)
			res=res*10+r;
	}
	printf("After=%d\n",res);
}
