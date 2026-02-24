#ifndef __ADC_DEFINES_H__
#define __ADC_DEFINES_H__

#include "types.h"

#define FOSC     12000000UL
#define CCLK     (5 * FOSC)
#define PCLK     (CCLK / 4)
#define ADCCLK   3000000UL
#define CLKDIV   ((PCLK / ADCCLK) - 1)

// ADCR bits
#define CLKDIV_BITS    8
#define PDN_BIT        21
#define ADC_START_BIT  24

// ADDR bits
#define RESULT_BITS    6
#define DONE_BIT       31

// ADC channels
#define ADC_CH0  0
#define ADC_CH1  1
#define ADC_CH2  2
#define ADC_CH3  3
#define ADC_CH4  4
#define ADC_CH5  5
#define ADC_CH6  6
#define ADC_CH7  7

#endif
