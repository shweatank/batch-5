#include <lpc21xx.h>
#include "adc_defines.h"
#include "types.h"
#include "pin_connect_block.h"
#include "pin_function_defines.h"
#include "delay.h"


void Init_ADC() {
	ADCR |= ((1 << PDN_BIT) | (CLKDIV << CLKDIV_BITS));
	//cfg port0.27 in as analog input pin
	CfgPortPinFunc(0,27,PIN_FUNC2);
}

void Read_ADC(u32 chno,u32 *temperature,f32 *eAR) {
	u32 adcDval = 0;
	//clear the previous inputs
	ADCR &= ~(255);
	//update req channel selection
	//start analog sampling and conversion
	ADCR |= 1 << chno | 1 << ADC_START_BIT;
	delay_us(3);
	while(((ADDR >> DONE_BIT) & 1) == 0);
	ADCR &= ~(1 << ADC_START_BIT); // to off adc
	adcDval = (ADDR >> RESULT_BITS) & 1023; //for 10 bits
	*eAR = (adcDval * (3.3/1023)); //ref vltg / resolution bits
	*temperature = *eAR * 100;
}
