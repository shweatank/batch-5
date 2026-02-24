#ifndef __DEFINES_H__
#define __DEFINES_H__

// Basic bit macros
#define SETBIT(WORD,BITPOS)      ((WORD) |= (1U << (BITPOS)))
#define CLRBIT(WORD,BITPOS)      ((WORD) &= ~(1U << (BITPOS)))

// Write an 8-bit value into a byte-aligned field starting at BITPOS
#define WRITEBYTE(WORD,BITPOS,BITLEVEL) \
    ( (WORD) = ((WORD) & ~(0xFFU << (BITPOS))) | (((u32)(BITLEVEL) & 0xFFU) << (BITPOS)) )

#endif
