#include <stdio.h>
#include <string.h>

int main()
{
    char s[50] = "12345678901234567890";
    printf("%s\n",s);
    memcpy(s + 5, s, 15);
    printf("%s\n", s);

    char p[50] = "12345678901234567890";

    memmove(p + 5, p, 15);
    printf("%s\n", p);
}
