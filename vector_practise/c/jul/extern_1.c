#include <stdio.h>
extern int num;
void v2() {
	printf("In another file before modifying num: %d\n",num);
	num += 10;
	printf("In another file after modifying num: %d\n",num);
}

