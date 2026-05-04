#include<stdio.h>
#include<stdlib.h>
void main()
{
char *p=malloc(sizeof(char)*10);
scanf("%s",p);
printf("%s\n",p);
}
