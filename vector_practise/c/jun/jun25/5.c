#include <stdio.h>
int main() {
        int n;
        printf("Enter the number: ");
        scanf("%d",&n);
        for(int i = -n;i<=n;i++) {
		int num = (i < 0) ? -i : i;
		for(int j = 1;j<=(n+1)-num;j++) {
			printf("* ");
		}
		printf("\n");
	}
}


			
