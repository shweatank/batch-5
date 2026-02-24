#include <stdio.h>
int main() {
    char ch;
    scanf("%c",&ch);
    if(ch == '-') {
        printf("invalid input"); 
        return 0;
    }
    for(int i = 7;i>=0;i--) {
        printf("%d",(ch & (1 << i)));
    }
}
