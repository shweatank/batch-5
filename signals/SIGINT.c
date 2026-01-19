#include"header.h"
void ISR(int n)
{
printf("before sleep\n");
sleep(10);
printf("after sleep\n");
}
void main()
{
printf("hi\n");
signal(SIGINT,ISR);
while(1);
}
