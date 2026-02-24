#include<stdio.h>
#include<stdlib.h>
struct demo
{
	int roll;
	char str[20];
};

struct demo* alloc_mem(struct demo *ptr)
{
	struct demo *temp;
	ptr=malloc(sizeof(*ptr));
	temp=ptr;
	while(!(*temp)%16)
	{
		free(ptr);
		ptr=malloc(sizeof(*ptr));
		temp=ptr;
	}
	return ptr;
}

int main()
{
	struct demo *var=NULL;
	var=alloc_mem(var);
	(!var) ? puts("memory allocation failure") : \
		printf("memory allocated sucussfully : %ld\n",&var);
			
}
