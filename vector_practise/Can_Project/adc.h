#ifndef __ADC_H__
#define __ADC_H__

#include "types.h"

void Init_ADC(void);
f32 Read_ADC(u8 chno);   // returns voltage (0..3.3V)

#endif
