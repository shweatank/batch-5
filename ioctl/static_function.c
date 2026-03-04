#include<stdio.h>
#include"static.h"

extern void function();

void (*p)(void);

void main()
{

	p=function;

//	printf("%d\n",i);

         p();
}
	 
