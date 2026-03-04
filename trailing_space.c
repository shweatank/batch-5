#include <stdio.h>
#include <string.h>

/* trim: remove trailing blanks, tabs, newlines */
int trim(char s[])
{
    int n;
    for (n = strlen(s)-1; n >= 0; n--)
        if (s[n] != ' ' && s[n] != '\t' && s[n] != '\n')
            break;

    s[n+1] = '\0';
    return n;
}

/* driver code */
int main(void)
{
    char str[100];

    printf("Enter a string: ");
    fgets(str, sizeof(str), stdin);

    printf("Before trim: \"%s\"\n", str);

    trim(str);

    printf("After trim:  \"%s\"\n", str);

    return 0;
}
