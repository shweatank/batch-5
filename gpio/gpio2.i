# 0 "gpio2.c"
# 0 "<built-in>"
# 0 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 0 "<command-line>" 2
# 1 "gpio2.c"
volatile unsigned int GPIO;

int main(void)
{
    volatile unsigned int i;

    while (1)
    {
        GPIO = 1;


        for(i = 0; i < 100000; i++);

        GPIO = 0;


        for(i = 0; i < 100000; i++);
    }
}
