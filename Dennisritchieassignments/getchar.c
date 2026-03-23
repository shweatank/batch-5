/*Verify that the expression getchar() != EOF is 0 or 1.i*/
#include <stdio.h>
void main() {
	int n;
	n=getchar()!=EOF;
	printf("value:%d\n",n);
   
}
