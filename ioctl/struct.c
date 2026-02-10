#include<stdio.h>
struct st
{
	  int b;
};
struct st *a;

void main()
{
	
	a=malloc(1);
        perror("malloc");
	a->b=1;
	printf("%d\n",a->b);

	printf("%ld\n",sizeof(*a));

}


