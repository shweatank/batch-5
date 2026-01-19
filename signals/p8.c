#include"header.h"
void SIGINT_ISR(int);
void SIGTERM_ISR(int);
void main()
{

signal(SIGINT,SIGINT_ISR);
signal(SIGTERM,SIGTERM_ISR);
while(1);

}
void SIGINT_ISR(int n)
{
printf("SIGINT is recieved\n");
}
void SIGTERM_ISR(int n)
{
printf("SIGTERM is recieved\n");
exit(0);
}
