#include <stdio.h>
int main() {
	int num;
	printf("Enter the number: ");
	scanf("%d",&num);
	char ch = 'A';
	for(int i=1;i<=num;i++) {
		for(int j=1;j<=num;j++) {
			if(i>=j) {
				printf("%c ",ch);
				ch++;
			}
		}
		printf("\n");
	}
}
