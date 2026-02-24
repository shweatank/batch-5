#include "types.h"
#include"delay.h"
void delayUs(u32 dlyus)
{
dlyus*=12;
while(dlyus--);
}
void delayMs(u32 dlyMs)
{
dlyMs*=12000;

while(dlyMs--);
                                                                                                                                                                  }
 
void delayS(u32 dlyS)
{							 
  dlyS *= 12000000;
while(dlyS--);

}
