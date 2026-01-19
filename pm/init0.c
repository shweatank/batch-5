#include"header.h"
void main()
{
char*argv[]={"init","0",NULL};
execvp(argv[0],argv);
}
