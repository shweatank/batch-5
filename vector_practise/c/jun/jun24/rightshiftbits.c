//Right shift of bits
#include <stdio.h>
int main() {
        int num,shiftbits;
        printf("Enter the number: ");
        scanf("%d",&num);
        printf("Enter the no.of bits to be right shifted: ");
        scanf("%d",&shiftbits);
        printf("%d\n",num >> shiftbits);
        return 0;
}
