#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
void main(){

	volatile unsigned int *GPIOSEL0,*GPIOSET0,*GPIOCLR0;
	FILE *fp=fopen("address","r");
	perror("fopen");
	fscanf(fp,"%p%p%p",GPIOSEL0,GPIOSET0,GPIOCLR0);
	printf("%p %p %p\n",GPIOSEL0,GPIOSET0,GPIOCLR0);
	int i=0;
	*GPIOSEL0|=(1<<0);
	*GPIOSET0|=(1<<0);
	while(1){
		*GPIOSET0=(1<<0);
		for(i=0;i<1000000;i++);
		*GPIOCLR0=(1<<0);
	}
}



	
