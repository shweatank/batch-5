#include <stdio.h> 
int num = 10;
void v2();
int main() {
	printf("in main before modifying num :%d\n",num);
	v2();
	num+= 10;
	printf("in main after modifying num :%d\n",num);
}

