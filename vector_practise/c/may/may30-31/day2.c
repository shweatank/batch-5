#include<stdio.h>
#define PI 3.14

void main() {
	int rad;
	float area,circum;
	printf("Enter the radius of circle: ");
	scanf("%d",&rad);
	area = PI*rad*rad;
	circum = 2*PI*rad;
	printf("area = %0.1f\n",area);
	printf("circumference = %0.1f\n",circum);
	return;
}
