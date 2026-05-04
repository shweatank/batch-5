#include<stdio.h>
void main()
{
FILE *fp;
fp=fopen("data","r");
if(fp==0)
printf("file not present\n");
else
printf("file is present\n");
fclose(fp);
}
