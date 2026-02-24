#include <stdio.h>
int main() {
        int n;
        printf("Enter the number: ");
        scanf("%d",&n);
        int flag = 1;
	char ch = 65+n;
        for(int i=-n;i<=n;i++) {
                flag = i; if(flag<0) flag = -flag;
		char c = ch;
                for(int j = 1;j<=(n+1);j++) {
			(flag+j > n) ? printf("%c ",c--) : printf("  ");
                }
		(i<0) ? ch-- : ch++;
        	printf("\n");
        }
}

