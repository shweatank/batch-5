#include<stdio.h>
#include<stdlib.h>
void main()
{
int *p[5],i;

for(i=0;i<5;i++)
p[i]=malloc(sizeof(char)*10);

for(i=0;i<5;i++)
scanf("%s",
