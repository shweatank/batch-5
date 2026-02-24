#include <stdio.h>
void v2() {
	printf("In another file before modifying num: %d",num);
	num += 10;
	printf("In another file after modifying num: %d",num);
}

