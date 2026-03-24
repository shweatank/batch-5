#include<stdio.h>
void main()
{
int a=23,b=2188766,c=5999;
//a>b?(a>c?printf("%d is greater\n",a):printf("%d is greater\n",c)):(b>c?printf("%d is greater\n",b):printf("%d is grater\n",c));
/*if(a>b&& a>c)
printf
else if(b>a && b>c)
printf
else if(c>a && c>b)
printf("");*/
if(a>b)
{
if(a>c)
printf("%d is greater\n",a);
else
printf("%d is greater\n",c);
}
else
{
if(b>c)
printf("%d is greater\n",b);
else
printf("%d is greater\n",c);

}

}
