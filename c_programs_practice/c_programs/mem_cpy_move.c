#include <stdio.h>
#include <string.h>

int main() {
    char str1[] = "stackoverflow";
    char str2[] = "stackoverflow";

    // Scenario: Source and Destination overlap
    // Source: index 0 ("stackov...")
    // Destination: index 5 ("...overflow")
    // Size: 7 bytes
    
    // 1. Using predefined memcpy (Undefined Behavior for overlap)
    memcpy(str1, str1+3, 7); 
    printf("Result of memcpy:  %s\n", str1);

    printf("%p %p\n",str1,str1+2);
    // 2. Using predefined memmove (Defined Behavior for overlap)
    memmove(str2, str2+3, 7);
    printf("Result of memmove: %s\n", str2);

    return 0;
}

