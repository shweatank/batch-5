#include<LPC21xx.h>

void adc_init(void)
{
	const int a[]={4,19,9,4,4};
	u32 clkdiv=0;
	clkdiv=a[VPBDIV};
	PINSEL1|=0x15400000;
	ADCR=0x00200000|(clkdiv<<8);
}

#define DONE (ADDR>>31)&1

u32 adc_read(u8 ch_num)
{
	u32 result=0;
	ADCR |= (1<<ch_num);   // select the channel number
	ADCR |= (1<<24);	// start ADC
	while(DONE==0);		//wait for ADC to complete
	ADCR ^= (1<<24);	// stop ADC
	ADCR ^= (1<<ch_num); 	// deselect adc channel
 	result = (ADDR>>6)&0x3FF;
	return result;
}
