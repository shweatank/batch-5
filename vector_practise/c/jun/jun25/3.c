#include <stdio.h>
int main() {
        int n;
        printf("Enter the number: ");
        scanf("%d",&n);
	int flag = 1;
        for(int i = 1;i<=n;i++) {
		int flag = i%2;
		for(int j = 1;j<=i;j++) {
			printf("%d ",flag);
			flag = !flag;
		}
		printf("\n");
	}
}
