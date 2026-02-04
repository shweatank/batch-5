#include<stdio.h>
void swap(char *);
int main()
{
	int n=0x12345678;
	int i;
	char *p=(char *)&n;
	for(i=0;i<sizeof(n);i++)
	{
		swap(p);
		p++;
	}
	printf("%x\n",n);
}
void swap(char *p)
{
	*p=(((*p>>4)&0x0f)|((*p<<4)&0xf0));
}

