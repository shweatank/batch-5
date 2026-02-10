#include <stdio.h>

int main() {
    
    // Normal Variable
    int var = 10;
    
    // Pointer Variable ptr that 
    // stores address of var 
    int* ptr = &var;
    
    // Directly accessing ptr will
    // give us an address
    printf("%d", ptr);
    
    return 0;
}
