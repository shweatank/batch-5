#include<stdio.h>
void main()
{
float f;
printf("enter the val\n");
scanf("%f",&f);
int *p=(int *)&f;
int pos;

for(pos=31;pos>=0;pos--)
printf("%d ",*p>>pos&1);
}
