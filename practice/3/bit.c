#include<stdio.h>
void main(){

int num;
printf("Enter num\n");
scanf("%d",&num);
int c=0,n=num;
while(n){
n=n&(n-1);
c++;
}
printf("No.of set bits are %d\n",c);
c=0;

n=num;
n=~n;
while(n){
n=n&(n-1);
c++;
}
printf("No.of clear bits are %d\n",c);
}
