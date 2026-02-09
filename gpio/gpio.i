# 0 "gpio.c"
# 0 "<built-in>"
# 0 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 0 "<command-line>" 2
# 1 "gpio.c"
volatile unsigned int GPIO;

int main(void)
{
    while (1)
    {
        GPIO = 1;
        GPIO = 0;
    }
}
