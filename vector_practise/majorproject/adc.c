#include<lpc21xx.h>
#include"types.h"
#include"pin_connect_block.h"
#include"pin_function_define.h"
#include"adc_defines.h"
#include"delay.h"
void Init_ADC(void)
{
	ADCR|=((1<<PDN_BIT)|(CLKDIV<<CLKDIV_BITS));
	CfgPortPinFunc(0,27,PINFUN2); //CONFIGURE P0.27 FOR  ADC FUNC
}
/* CALCULATE ANALOG VALUE*/
float Read_ADC(u32 chNO)
{
	f32 ear;
	u32 Dval;
	ADCR&=0XFFFFFF00;
	ADCR|=1<<chNO|1<<ADC_START_BIT;
	delayUs(3);
	while(((ADDR>>DONE_BIT)&1)==0);
	ADCR&=~(1<<ADC_START_BIT);
	Dval=(ADDR>>RESULT_BITS)&1023;
	ear=(Dval*(3.3/1023));
	return ear;
}

