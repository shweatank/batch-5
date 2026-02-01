#include"header.h"
unsigned int *gpio_pin,*set,*clr;
#define LED (1<<0)
int main()
{
FILE*fp=fopen("address.txt","r");
if(fp==0)
{	
printf("no file\n");
return 0;
}
fscanf(fp,"%x%x%x",gpio_pin,set,clr);
printf("%x %x %x",gpio_pin,set,clr);
fclose(fp);
int i;
*gpio_pin=(1<<0);
while(1)
{
*clr=(1<<0);
for(i=0;i<100000;i++);
*set=(1<<0);
for(i=0;i<100000;i++);
}
}
