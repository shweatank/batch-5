#include <stdio.h>
int main() {
        int num;
        printf("Enter the number: ");
        scanf("%d",&num);
        int a;
        for(int i = -num;i<=num;i++) {
	       	if(i < 0) a = -i;
        	else a = i;
                for(int j = 1;j<=num+1;j++) {
                       if(j > a) printf(" *");
		       else printf(" ");
                }
                printf("\n");
        }
}

