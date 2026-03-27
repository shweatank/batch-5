#include<lpc21xx.h>
#define DONE (ADDR>>31)&1
void adc_init(void)
{
	PINSEL1|=0x04000000;  // for enabling the pin configuration pin 29


	ADCR=0X00200400;   // for power on and setting the clock 
}

unsigned short int adc_read(char ch_num)
{
	   unsigned short int result=0;

	   ADCR |=1<<ch_num;  // select the channel number

	   ADCR |= 1<<24;   //start the adc;
	
	     while(DONE==0);

	     ADCR^=1<<24;   // stop the adc
			   
	     result=(ADDR>>16)&0X3FF;

	     return result;
}








