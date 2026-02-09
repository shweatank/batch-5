#include <stdio.h>

void printTable(int n) { 
    for (int i = 1; i <= 10; ++i) 
        printf("%d * %d = %d\n", n, i, n * i);
}

int main() {
    int n = 2;  
    printTable(n);
    return 0;
}
printf("hello world");
