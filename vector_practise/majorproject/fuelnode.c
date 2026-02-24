#include<lpc21xx.h>
#include"can.h"
#include"adc.h"
#include"lcd.h"
#include"lcd_defines.h"


f32 aR;
u32 fuelval=0;
CANF txf;
int main()
{ 
	Init_ADC();
	Init_CAN1();
	Init_LCD();
	txf.ID=2;
	txf.vbf.RTR=0;
	txf.vbf.DLC=1;
	while(1)
	{
		aR=Read_ADC(0);	//analogResult
	fuelval=((aR-1.95)/(3.29-1.95))*100;
	//fuelval=aR;
		if(fuelval>100)
		    fuelval=100;
		//txf.Data1=fuelval;
		CmdLCD(GOTO_LINE1_POS0);
		F32LCD(fuelval,3);


		//CAN1_Tx(txf);
		//delayMs(1000);
	}
}
