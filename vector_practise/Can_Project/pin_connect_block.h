#ifndef __PIN_CONNECT_BLOCK_H__
#define __PIN_CONNECT_BLOCK_H__

#include "types.h"

// Configure pin function for LPC21xx:
// port 0, pin 0-31 (only port0 used here)
void CfgPortPinFunc(u8 port, u8 pin, u8 func);

#endif
