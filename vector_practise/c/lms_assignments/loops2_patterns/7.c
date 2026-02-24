#include <stdio.h>
int main() {
	int num;
	printf("Enter the number: ");
	scanf("%d",&num);
	int a = 0;
	for(int i = -num;i<=num;i++) {
		if(i < 0) a = -1*i;
		else a = i;
		for(int j = 1;j<=a+1;j++) {
			printf("* ");
		}
		printf("\n");
	}
}

