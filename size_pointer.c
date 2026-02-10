#include <stdio.h>

int main() {
    int *ptr1;
    char *ptr2;
    
    // Finding size using sizeof()
    printf("%zu\n", sizeof(ptr1));
    printf("%zu", sizeof(ptr2));
    
    return 0;
}
