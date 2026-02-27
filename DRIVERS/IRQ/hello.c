#include<stdio.h>
void main(){

printf("Hello world\n");
int *p;
*p=20;
while(*p){
*p--;
}
printf("*p=%d\n",*p);
}
