#include <stdio.h>
int main() {
	int num = 10;
	int *pnum = &num;
	printf("pnum = %p\n",pnum);
	printf("*pnum = %d\n",*pnum);

	printf("&num = %p\n",&num);

	printf("&pnum = %p\n",&pnum);

	int **ppnum = &pnum;
	printf("ppnum = %p\n",ppnum);
	printf("**ppnum = %d\n",**ppnum);
	printf("*ppnum = %p\n",*ppnum);

	printf("&ppnum = %p\n",&ppnum);
}
