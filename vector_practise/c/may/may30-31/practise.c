
#include <stdio.h>
#define pi 3.14
void main() {
	int rad;
	float area,circumf;
	printf("Enter the radius: ");
	scanf("%d",&rad);
	area = pi*rad*rad;
	circumf = 2*pi*rad;
	printf("Area = %0.1f\n",area);
	printf("Circumference = %0.1f\n", circumf);
	return;
}
