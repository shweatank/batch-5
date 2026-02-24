#include <lpc21xx.h>
#include "types.h"
#include "rtc.h"
#include "lcd.h"
#include "interrupt.h"
#include "interrupt_defines.h"
#include "kpm.h"
#include "adc.h"
#include "defines.h"
#include "rtc_defines.h"

s32 hour,min,sec,date,month,year,day,alarm_hour = -1,alarm_min = -1;
s32 eint_request,alarm_stop;
u32 temperature,chno = 0; //default is also 0
f32 eAR;
// Main function
int main()
{
	// Initialize RTC 
	Init_RTC();
	// Initialize the LCD
	Init_LCD();
	CGRAM_Symbols();
	//Initialize the KPM
	Init_KPM();
	//Initialize ADC
	Init_ADC();
	// Initialize the Interrupt
	interrupt_configuration();
	
	// Set the initial time (hours, minutes, seconds)
	SetRTCTimeInfo(9,54,32);
	SetRTCDateInfo(1,11,2025);
	SetRTCDay(6);
	
	while (1) 
	{
		// Get and display the current time on LCD
		GetRTCTimeInfo(&hour,&min,&sec);
		GetRTCDay(&day);
		GetRTCDateInfo(&date,&month,&year);
//		DisplayRTCTime(HOUR,MIN,SEC);
//		DisplayRTCDay(DOW);
//		DisplayRTCDate(DOM,MONTH,YEAR);
		DisplayRTCTime(hour,min,sec);
		DisplayRTCDay(day);
		DisplayRTCDate(date,month,year);
		Read_ADC(chno,&temperature,&eAR);
		Display_temperature(temperature);
		if(eint_request) {
			menu_display(&alarm_hour,&alarm_min);
		}
		if(HOUR == alarm_hour && MIN == alarm_min && alarm_stop == 0) {
			ring_alarm(alarm_hour,alarm_min);
		}
	}
}
