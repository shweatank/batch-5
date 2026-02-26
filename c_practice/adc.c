#include<lpc21xx.h>

#define FOSC 1200000
#define CCLK (FOSC*5)
#define PCLK (CCLK/4)
#define ADCCLK 3000000
#define CLKDIV ((PCLK/ADCCLK)-1)

#define PDN 21
#define ADC_START_BIT 24
#define CLK_DIV_BITS  8
#define RESULT_BIT 6
#define DONE_BIT 31

int adcval;
float ear;

void init_adc(void)
{
	ADCR|=(1<<PDN) | (CLKDIV<<CLKDIV_BITS);
	configportpin(0,24,1);
}

void read_adc(int chno,int *adcval,float *ear)
{
	ADCR&=0XFFFFFF00;
	ADCR|=1<<chno |1<<ADC_START_BIT;
	delayus(3);
	while(((ADDR>>DONE_BIT)&1)==0);
	ADCR&=~(1<<ADC_START_BIT);
	*adcval=(addr>>result_bits)&1023;
	*ear=(*adcval*(3.3/1023);
}
int main()
{
	init_adc();
	read_adc(27,&adcval,&ear);
}
