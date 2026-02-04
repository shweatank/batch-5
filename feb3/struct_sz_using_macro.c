#include<stdio.h>

typedef struct sample
{
	int data;
	char choice;
}samp;


#define STRUCT_SIZE(VAR) (sizeof(VAR))

int main()
{
	samp s1;
	printf("printing struct size using macro : %ld\n",STRUCT_SIZE(s1));
	return 0;
}
