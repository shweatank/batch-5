#include<stdio.h>
void print_binary(unsigned int i)
{
	for(int pos=31;pos>=0;pos--)
	printf("%d",i>>pos&1);


}
void main()
{
	unsigned int i=15;
	
	print_binary(i);

	i^=0xffffffff;
	
	printf("\n");

	print_binary(i);

}


