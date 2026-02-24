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
	int sum = (1 << bitPosition);
        num = (sum & num) ? num-sum : num+sum;
	printf("The result number after toggling at given bit position is: %d\n",num);
}

