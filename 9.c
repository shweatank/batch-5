#include<stdio.h>
struct st
{
	int a;
	int b;
	int c;
	int d;
	char op;
};

void main()
{
	struct st v;

	v.a=1;
	v.b=2;
	v.c=3;
	v.d=4;
	v.op='+';


	char s[10];

	sprintf(s,"%d%d%d%d",v.a,v.b,v.c,v.d);

	int *var;

	var=(int *)s;
	printf("%d\n",*var);
	printf("%s\n",s);

}
