#include <stdio.h>
int main() {
        int num;
        printf("Enter the number: ");
        scanf("%d",&num);
        int ch = num;
        for(int i=1;i<=num;i++) {
                for(int j=1;j<=num;j++) {
                        if(i+j <= (num+1)) {
				if(i%2 == 0) {
					printf("* ");
				}
				else {
					printf("%d ",ch);
				}
			}
		}
		ch--;
		printf("\n");
	}
}
