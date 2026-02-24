#include <stdio.h>
int main() {
        int n;
        printf("Enter the number: ");
        scanf("%d",&n);
        int flag = 1;
        for(int i =-n;i<=n;i++) {
                flag = i; if(flag<0) flag = -flag;
                for(int j = 1;j<=(flag+1);j++) {
                        printf("* ");
                }
                printf("\n");
        }
}

