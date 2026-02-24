#include <LPC21xx.h>
#include "adc_defines.h"
#include "adc.h"
#include "types.h"
#include "pin_connect_block.h"
#include "pin_function_defines.h"
#include "delay.h"

void Init_ADC(void)
{
    // Power up ADC and set clock divider
    ADCR = (1U << PDN_BIT) | (CLKDIV << CLKDIV_BITS);

    // Configure P0.27 as AD0.0 (fuel gauge sensor input)
    CfgPortPinFunc(0, 27, PIN_FUNC2);
}

f32 Read_ADC(u8 chno)
{
    u32 adcDval;
    f32 eAR;

    // clear previous channel selection (bits0..7)
    ADCR &= ~0xFFU;

    // select channel and start conversion
    ADCR |= (1U << chno) | (1U << ADC_START_BIT);

    delay_us(3);

    // wait for DONE bit
    while (((ADDR >> DONE_BIT) & 1U) == 0U)
        ;

    // stop conversion
    ADCR &= ~(1U << ADC_START_BIT);

    // read 10-bit result
    adcDval = (ADDR >> RESULT_BITS) & 0x3FFU;

    // convert to voltage (assuming Vref = 3.3V)
    eAR = ((f32)adcDval * 3.3f) / 1023.0f;

    return eAR;
}
