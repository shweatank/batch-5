#include <stdio.h>
int main() {
        int num;
        printf("Enter the number: ");
        scanf("%d",&num);
	int flag1,flag2;
        for(int i = -num;i<=num;i++) {
		flag1 = i; if(flag1<0) flag1 = -flag1;
                for(int j=-num;j<=num;j++) {
			flag2 = j; if(flag2<0) flag2 = -flag2;
                        if(flag1+flag2 >= num) {
                                printf("* ");
                        }
                        else {
                                printf("  ");
                        }
                }
                printf("\n");
        }
}
