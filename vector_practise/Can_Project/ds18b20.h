#ifndef __DS18B20_H__
#define __DS18B20_H__

#include "types.h"

void Init_DS18B20(void);
u32 ReadTemp(void);     // returns raw 12-bit DS18B20 temperature

#endif
