#include <stdio.h>
int main() {
        int n;
        printf("Enter the number: ");
        scanf("%d",&n);
	int flag = 1;
        for(int i = n;i>=1;i--) {
		for(int j = 1;j<=i;j++) {
			(flag) ? printf("%c ",64+j) : printf("* ");
		}
	flag = !flag;
	printf("\n");
	}
}

