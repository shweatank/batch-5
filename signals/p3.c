#include"header.h"
void main()
{
printf("before raise..\n");
raise(SIGINT);
printf("after raise..\n");
}
