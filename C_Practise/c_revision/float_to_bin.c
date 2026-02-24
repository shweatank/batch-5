#include <stdio.h>

union tag {
	float f;
	int n;
};

int main() {
	union tag var;
	printf("Enter the float num: ");
	scanf("%f",&var.f);
	for(int i = 31;i>=0;i--) {
		printf("%d",((var.n >> i)&1));
	}
	printf("\n");
}


