#include"header.h"
void ISR(int n)
{
printf("SIGINT is handled\n");
}
void main()
{
printf("waiting for signal\n");
signal(SIGINT,ISR);
pause();
printf("SIGINT is recievd\n");
}
