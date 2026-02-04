#include<stdio.h>
void printbinary(char *);
int main()
{
	float f;
	puts("enter the float number");
	scanf("%f",&f);
	char *p=(char *)&f;
	for(int i=0;i<sizeof(f);i++)
	{
		printbinary(p);
		p++;
	}
	puts("");
}
void printbinary(char *p)
{
	for(int i=7;i>=0;i--)
	{
		if(((*p>>i)&1)==1)
			printf("1 ");
		else
			printf("0 ");
	}
}
