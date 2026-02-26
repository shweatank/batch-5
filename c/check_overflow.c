/*#include<stdio.h>
#include<stdint.h>
void main()
{
uint8_t a,b;
a=12;
b=12;
if(a^b>=0)
printf("Overflow\n");
else
printf("Not an Overflow\n");
}*/


#include<stdio.h>
#include<stdint.h>
void main()
{
uint8_t a,b;
a=128;
b=128;
//printf("Enter a and b:\n");
//scanf("%d%d",&a,&b);
if((a+b)>255)
printf("Overflow\n");
else
printf("Not an overflow\n");
}
