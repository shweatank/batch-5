#include <stdio.h>
union tag {
	float m1;
	int m2;
};
void main() {
	int bit;
	union tag var;
	printf("Enter a real data: ");
	scanf("%f", &var.m1);
	for(bit = 31;bit >= 0; bit--) {
		printf("%d",(var.m2 >> bit) &1);
	}
	printf("\n");
	
}
