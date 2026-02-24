#ifndef RTC_DEFINES_H
#define RTC_DEFINES_H

// System clock and peripheral clock Macros
#define FOSC 12000000
#define CCLK (5*FOSC)
#define PCLK (CCLK/4)

// RTC Macros
//#define _LPC2148_

#ifndef _LPC2148_
#define CLKSRC_VAL	 (1 << 4)
#define PREINT_VAL ((int)((PCLK / 32768) - 1))
#define PREFRAC_VAL (PCLK - ((PREINT_VAL+1)*32768))
#else
#define CLKSRC_VAL	 (0 << 4)
#define PREINT_VAL   PREINT
#define PREFRAC_VAL  PREFRAC
#endif

#define RTC_ENABLE (1<<0) 
#define RTC_RESET  (1<<1)

#define ALARM_START_PIN  5
#define ALARM_STOP_SWAL  6

#endif
