#include<stdio.h>
void main()
{
int num=10;
char *p=(char *)&num;
if(*p==10)
printf("little \n");
else
printf("big\n");
}

