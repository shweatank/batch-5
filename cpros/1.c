#include <stdio.h>

void reverse(char s[])
{
    int i, j;
    char temp;

    for(i = 0, j = strlen(s)-1; i < j; i++, j--)
    {
        temp = s[i];
        s[i] = s[j];
        s[j] = temp;
    }
}

void itob(int n, char s[], int b)
{
    int i = 0, remainder;

    do
    {
        remainder = n % b;

        if(remainder > 9)
            s[i++] = remainder - 10 + 'A';   // for hex digits
        else
            s[i++] = remainder + '0';

        n = n / b;

    } while(n > 0);

    s[i] = '\0';

    reverse(s);
}

int main()
{
    char s[20];

    itob(23, s, 16);

    printf("Result = %s\n", s);

    return 0;
}
