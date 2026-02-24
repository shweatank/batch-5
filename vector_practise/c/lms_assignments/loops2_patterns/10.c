#include <stdio.h>
int main() {
        int num;
        printf("Enter the number: ");
        scanf("%d",&num);
	int J;
        for(int i = num+1;i>=1;i--) {
                for(int j=-num;j<=num;j++) {
			J = j; if(J < 0) J = -J;
                        if(i+J > num) {
                                printf("%d ",(num+1)-J);
                        }
                        else {
                                printf("  ");
                        }
                }
                printf("\n");
        }
}
