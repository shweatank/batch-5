#include <stdio.h>
int main() {
        int num,bitPosition;
        printf("Enter the number: ");
        scanf("%d",&num);
        printf("Enter the bit position to be checked: ");
        scanf("%d",&bitPosition);
        if(bitPosition > 31) {
                printf("Please enter the number between 0 to 31");
                return 0;
        }
        ((1<<bitPosition) & num) ? printf("The bit at %d position is set\n",bitPosition) : printf("The bit at %d position is clear\n",bitPosition);
}

