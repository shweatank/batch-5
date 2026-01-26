#include<stdio.h>
#include<stdlib.h>

void main()
{
FILE *fp=fopen("/dev/my_file","w");
char ch[10]="hello";

fwrite(ch,10,1,fp);
fclose(fp);

fp=fopen("/dev/my_file","r");
fread(ch,10,1,fp);

printf("The data is %s\n",ch);
fclose(fp);





}
