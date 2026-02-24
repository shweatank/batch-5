#include "adc.h"
#include "adc_defines.h"

void init_adc(void) {
	ADCR |= 1 << PDN_BIT | (CLKDIV << CLKDIV_BITS);
	//configure p0.27 pins as analog input pin
	//PINSEL1 = 
}

void read_adc(u32 chno,u32 *dval,f32 *aval) {
	
	ADCR |= 1 <<chno | 1 << ADC_START_BIT;
	//delay for about 3-5Us
	while(((ADDR >> DONE_BIT)&1) == 0); //waiting till the completion of conversion
	
	ADCR &= ~(1 << ADC_START_BIT);
	*dval = (ADDR >> RESULT_BITS)&1023; //1023 bcoz its 10bit ADC
	*aval = (*dval * (3.3/1023));
}
