//eint0_virq_test.c
#include "types.h"
#include <LPC21xx.h>
#include "pin_function_defines.h"
#include "pin_connect_block.h"
#define EINT0_VIC_CHN0 14
#define EINT0_LED      16 //p1.16
void eint0_isr(void) __irq;
u32 gCount;
main()
{
	//cfg p0.1 as EINT0 input pin
  CfgPortPinFunc(0,1,EINT0_0_1);	
	//cfg VIC peripheral:
	//1. cfg eint0 interrupt type as irq
	//   def is irq
	//   VICIntSelect = 0;
	//2. enable eint0 in VIC 
	VICIntEnable = 1<<EINT0_VIC_CHN0;
	//3. cfg v.irq slot for higest priority
	VICVectCntl0 = (1<<5)|EINT0_VIC_CHN0;
	//4. load eint0_isr addr in reg
	VICVectAddr0 = (u32 )eint0_isr;
	
	//cfg External Interrupt periheral
	//5.enable eint0 ,default all are enabled
	//EXTINT = 0;
	//6. cfg eint0 as edge triggered interrupt
	EXTMODE = 1<<0;
	//7. cfg eint0 as falling edge triggering
	// EXTPOLAR=0;
	
	//cfg p1.16 as gpio output pin for eint0 status led
	IODIR1 |= 1<<EINT0_LED;
	
	while(1)
	{
		//some normal/regular operation,running
		gCount++;
	}
}	

void eint0_isr(void) __irq
{
	IOPIN1 ^= 1<<EINT0_LED;
	//clear eint0 status in External Interrupt Peripheral
	EXTINT=1<<0;
	//clear eint0 status in VIC peripheral
	VICVectAddr=0;
}
