#include "rtc_defines.h"
#ifndef _LPC2148_
#include <lpc21xx.h>
#else
#include <lpc214x.h>
#endif

#include "types.h" 
#include "delay.h"
#include "defines.h"
#include "lcd_defines.h"
#include "lcd.h"
#include "kpm.h"
#include "interrupt_defines.h"

// Array to hold names of days of the week
s8 week[][4] = {"SUN","MON","TUE","WED","THU","FRI","SAT"};
extern s32 alarm_stop;
/*
Initialize the Real-Time Clock (RTC)
This function disables the RTC, sets the prescaler values, 
and then enables the RTC.
*/
void Init_RTC(void) 
{
  // Disable and reset the RTC
  CCR = RTC_RESET;
  // Set prescaler integer and fractional parts
  PREINT = PREINT_VAL;
	PREFRAC = PREFRAC_VAL;
  // Enable the RTC
	CCR = RTC_ENABLE | CLKSRC_VAL; 
}

/*
Get the current RTC time
hour Pointer to store the current hour
minute Pointer to store the current minute
second Pointer to store the current second
*/
void GetRTCTimeInfo(s32 *hour, s32 *minute, s32 *second)
{
	*hour = HOUR;
	*minute = MIN;
	*second = SEC;
}

/*
Display the RTC time on LCD
hour value (0–23)
minute value (0–59)
second value (0–59) seperated by ':'
*/
void DisplayRTCTime(u32 hour, u32 minute, u32 second)
{
	CmdLCD(GOTO_LINE1_POS0);
	CharLCD((hour/10) + '0');
	CharLCD((hour%10) + '0');
	CharLCD(':');
	CharLCD((minute/10) + '0');
	CharLCD((minute%10) + '0');
	CharLCD(':');
	CharLCD((second/10) + '0');
	CharLCD((second%10) + '0');
}

/*
Get the current RTC date
day Pointer to store the current date (1–31)
month Pointer to store the current month (1–12)
year Pointer to store the current year (four digits)
*/
void GetRTCDateInfo(s32 *day, s32 *month, s32 *year)
{
	*day = DOM;
	*month = MONTH;
	*year = YEAR;
}

/*
Display the RTC date on LCD
Day of month (1–31)
Month (1–12)
Year (four digits) and seperated by '/'
*/
void DisplayRTCDate(u32 day, u32 month, u32 year)
{
	CmdLCD(GOTO_LINE2_POS0);
	CharLCD((day/10) + '0');
	CharLCD((day%10) + '0');
	CharLCD('/');
	CharLCD((month/10) + '0');
	CharLCD((month%10) + '0');
	CharLCD('/');
	U32LCD(year);
}

/*
Set the RTC time
Hour to set (0–23)
Minute to set (0–59)
Second to set (0–59)
*/
void SetRTCTimeInfo(u32 hour, u32 minute, u32 second)
{
	HOUR = hour;
	MIN = minute;
	SEC = second;
}

/*
Set the RTC date
day of month to set (1–31)
month to set (1–12)
year to set (four digits)
*/
void SetRTCDateInfo(u32 day, u32 month, u32 year)
{
	DOM = day;
	MONTH = month;
	YEAR = year;
}

/*
Get the current day of the week
dow Pointer to store Day of Week (0=Sunday, ..., 6=Saturday)
*/
void GetRTCDay(s32 *dow)
{
	*dow = DOW;
}

/*
Display the current day of the week on LCD
dow (Day of Week) (0=Sunday, ..., 6=Saturday)
*/
void DisplayRTCDay(u32 dow)
{
	CmdLCD(GOTO_LINE1_POS0 + 11);
  StrLCD(week[dow]);
}

/*
Set the day of the week in RTC
Day of Week to set (0=Sunday, ..., 6=Saturday)
*/
void SetRTCDay(u32 dow)
{
	DOW = dow;
}

int max_days_in_month(u32 month,u32 year) {
	switch(month) {
		case 2:
			return (year % 400 == 0 || (year % 4 == 0 && year % 100 != 0)) ? 29 : 28;
		
		case 1:
		case 3:
		case 5:
		case 7:
		case 8:
		case 10:
		case 12:
			return 31;
		
		case 4:
		case 6:
		case 9:
		case 11:
			return 30;
		
		default:
			return 0;
	}
}

void edit_rtc_info(void) {
	s32 num;
	u8 key;
	loop: CmdLCD(CLEAR_LCD);
				StrLCD("1.H 2.M 3.S 4.D");
				CmdLCD(GOTO_LINE2_POS0);
				StrLCD("5.D 6.M 7.Y 8.E");
				
				key = KeyScan();  // Wait for user input
				switch (key) {
					case '1':  // Hour
						CmdLCD(CLEAR_LCD);
						StrLCD("Enter Hour:");
						ReadNum(&num, &key);
						if (num < 0 || num > 23) {
							CmdLCD(GOTO_LINE2_POS0);
							StrLCD("Invalid Hour");
						} 
						else {
							HOUR = num;
							CmdLCD(GOTO_LINE2_POS0);
							StrLCD("Hour Saved");
						}
						delay_ms(500);
						break;

					case '2':  // Minute
						CmdLCD(CLEAR_LCD);
						StrLCD("Enter Min:");
						ReadNum(&num, &key);
						if (num < 0 || num > 59) {
							CmdLCD(GOTO_LINE2_POS0);
							StrLCD("Invalid Min");
						} 
						else {
							MIN = num;
							CmdLCD(GOTO_LINE2_POS0);
							StrLCD("Min Saved");
						}
						delay_ms(500);
						break;

					case '3':  // Second
						CmdLCD(CLEAR_LCD);
						StrLCD("Enter Sec:");
						ReadNum(&num, &key);
						if (num < 0 || num > 59) {
							CmdLCD(GOTO_LINE2_POS0);
							StrLCD("Invalid Sec");
						} 
						else {
							SEC = num;
							CmdLCD(GOTO_LINE2_POS0);
							StrLCD("Sec Saved");
						}
						delay_ms(500);
						break;

					case '4':
						CmdLCD(CLEAR_LCD);
						StrLCD("Enter DOW: ");
						ReadNum(&num,&key);
						if(num < 0 || num > 6) {
							CmdLCD(GOTO_LINE2_POS0);
							StrLCD("Invalid DOW");
						}
						else {
							DOW = num;
							CmdLCD(GOTO_LINE2_POS0);
							StrLCD("DOW Saved");
						}
						delay_ms(500);
						break;
							
						case '5':
						CmdLCD(CLEAR_LCD);
						StrLCD("Enter DOM: ");
						ReadNum(&num,&key);
						if(num < 0 || num > (max_days_in_month(MONTH,YEAR))) {
							CmdLCD(GOTO_LINE2_POS0);
							StrLCD("Invalid DOM");
						}
						else {
							DOM = num;
							CmdLCD(GOTO_LINE2_POS0);
							StrLCD("DOM Saved");
						}
						delay_ms(500);
						break;
						
						case '6':
						CmdLCD(CLEAR_LCD);
						StrLCD("Enter Month: ");
						ReadNum(&num,&key);
						if(num < 0 || num > 12) {
							CmdLCD(GOTO_LINE2_POS0);
							StrLCD("Invalid Month");
						}
						else {
							MONTH = num;
							CmdLCD(GOTO_LINE2_POS0);
							StrLCD("Month Saved");
						}
						delay_ms(500);
						break;
							
						case '7':
						CmdLCD(CLEAR_LCD);
						StrLCD("Enter Year: ");
						ReadNum(&num,&key);
						if(num < 400 || num > 4000) {
							CmdLCD(GOTO_LINE2_POS0);
							StrLCD("Invalid Year");
						}
						else {
							YEAR = num;
							CmdLCD(GOTO_LINE2_POS0);
							StrLCD("Year Saved");
						}
						delay_ms(500);
						break;
						
						case '8':
							return; //exit option
				}
				goto loop;
}

void set_alarm_time(s32 *hour,s32 *minutes) {
	s32 hr,min;
	u8 key;
	CmdLCD(CLEAR_LCD);
	CmdLCD(DSP_ON_CUR_ON);
	StrLCD("HH:MM");
	// Enter Hour
	loop1:CmdLCD(GOTO_LINE1_POS0);
				ReadNum(&hr, &key);
				if (hr < 0 || hr > 23) {
					CmdLCD(GOTO_LINE2_POS0);
					StrLCD("Invalid Hour");
					delay_ms(500);
					CmdLCD(GOTO_LINE2_POS0);
					StrLCD("            ");
					goto loop1;
				}
				else {
					*hour = hr;
					CmdLCD(GOTO_LINE1_POS0);
					if(hr <= 9) U32LCD(0);
					U32LCD(hr);
					CmdLCD(GOTO_LINE2_POS0);
					StrLCD("Hour Saved");
					delay_ms(500);
					CmdLCD(GOTO_LINE2_POS0);
					StrLCD("          ");
				}

	loop2:CmdLCD(GOTO_LINE1_POS0+3);
				ReadNum(&min, &key);
				if (min < 0 || min > 59) {
					CmdLCD(GOTO_LINE2_POS0);
					StrLCD("Invalid Minutes");
					delay_ms(500);
					CmdLCD(GOTO_LINE2_POS0);
					StrLCD("               ");
					goto loop2;
				}
				else {
					*minutes = min;
					CmdLCD(GOTO_LINE1_POS0+3);
					if(min <= 9) U32LCD(0);
					U32LCD(min);
					CmdLCD(GOTO_LINE2_POS0);
					StrLCD("Minutes Saved");
					delay_ms(500);
					CmdLCD(GOTO_LINE2_POS0);
					StrLCD("          ");
				}
	CmdLCD(DSP_ON_CUR_OFF);
	IOSET0 = 1 << ALARM_STOP_SWAL;
}

void ring_alarm(s32 alarm_hour,s32 alarm_min) {
	IODIR0 |= 1 << ALARM_START_PIN;
	IOSET0  = 1 << ALARM_START_PIN;
	CmdLCD(CLEAR_LCD);
	CmdLCD(GOTO_LINE1_POS0);
	StrLCD(" ");
	WriteLCD(2);
	while(MIN == alarm_min) {
		if(READBIT(IOPIN0,ALARM_STOP_SWAL) == 0) {
			alarm_stop = 1;
			break;
		}
	}
	IOCLR0  = 1 << ALARM_START_PIN;
}
