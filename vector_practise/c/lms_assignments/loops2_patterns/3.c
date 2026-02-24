#include <stdio.h>
int main() {
        int n;
        printf("Enter the number: ");
        scanf("%d",&n);
        char ch = 65;
        for(int i =1;i<=n;i++) {
                char c = ch;
                for(int j = 1;j<=n;j++) {
			if(i+j > n) {
                        	(j == n) ? printf("%c",c) : printf("%c*",c);
				c++;
			}
			else 
				printf(" ");
                }
                printf("\n");
        }
}

