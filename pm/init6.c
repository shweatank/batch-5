#include"header.h"
void main()
{
char*argv[]={"init","6",NULL};
execvp(argv[0],argv);
}
