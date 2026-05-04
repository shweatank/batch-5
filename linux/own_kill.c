#include<stdio.h>
#include<stdlib.h>
void main(int argc,char ** argv)
{
if(argc<3)
{
printf("usage:./a.out signum pid\n");
return;
}

kill(atoi (argv[1],atoi(argv[2])));
perror("kill");

}
