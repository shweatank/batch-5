#include <stdio.h> 
static int num = 10;
int main() {
	printf("in main before modifying num :%d",num);
	num+= 10;
	printf("in main after modifying num :%d",num);
	v2();
}

