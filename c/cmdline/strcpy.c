#include<stdio.h>
#include<string.h>
void main(int argc,char **argv)
{
if(argc<3)
{
printf("usage: ./a.out dest src\n");
return;
}
char d[10];
int i;
for(i=0;argv[2][i];i++)
d[i]=argv[2][i];
d[i]=argv[2][i];

printf("%s\n",d);

}
