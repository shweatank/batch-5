//get a char from file
#include<stdio.h>
void main()
{
FILE * fp=fopen("data","r");
if(fp==0)
{
printf("file not present\n");
return;
}
char ch;
while((ch=fgetc(fp))!=EOF)
printf("%c ",ch);
}
